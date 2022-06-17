#include "GP3_Boss_FastProjectileAttack.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UGP3_FastProjectileAttack::BeginPlay()
{
	Super::BeginPlay();

	BossAttackFunctions = GetOwner()->FindComponentByClass<UGP3_BossAttackFunctions>(); 

	//If BossAttackFunctions (UActorComponent) isn't found.. Add an instance of it
	if (BossAttackFunctions == nullptr)
		BossAttackFunctions = Cast<UGP3_BossAttackFunctions>(GetOwner()->AddComponentByClass(UGP3_BossAttackFunctions::StaticClass(), true, FTransform(), true));

	if (BossAttackFunctions != nullptr)
	{
		AActor* owner = GetOwner(); 
		ProjectileStartLocation = BossAttackFunctions->GetSceneComponentThroughTag(owner, "FastProjectileAttack");
	}
}

void UGP3_FastProjectileAttack::Attack_Implementation()
{
	if (!FailCheck())
		return; 

	//If ProjectileStartLocation isn't null -> Projectiles will start from this location relative to Owner
	//else -> Will use owners location
	FVector currentLocation;
	if (ProjectileStartLocation != nullptr)
		currentLocation = ProjectileStartLocation->GetComponentLocation(); 
	else
		currentLocation = GetOwner()->GetActorLocation();

	float angleDelta = 360 / 4;
	float currentAngle = StartAngle;

	for (size_t i = 0; i < 4; i++)
	{
		FVector p1Location = FVector(
			currentLocation.X + StartRadius * cos(FMath::DegreesToRadians(currentAngle - AngleBetweenProjectiles / 2)),
			currentLocation.Y + StartRadius * sin(FMath::DegreesToRadians(currentAngle - AngleBetweenProjectiles / 2)),
			currentLocation.Z
		);

		FVector p2Location = FVector(
			currentLocation.X + StartRadius * cos(FMath::DegreesToRadians(currentAngle + AngleBetweenProjectiles / 2)),
			currentLocation.Y + StartRadius * sin(FMath::DegreesToRadians(currentAngle + AngleBetweenProjectiles / 2)),
			currentLocation.Z
		);

		FVector p1Dir = FVector(
			cos(FMath::DegreesToRadians(currentAngle - AngleBetweenProjectiles / 2)),
			sin(FMath::DegreesToRadians(currentAngle - AngleBetweenProjectiles / 2)),
			0
		);

		FVector p2Dir = FVector(
			cos(FMath::DegreesToRadians(currentAngle + AngleBetweenProjectiles / 2)),
			sin(FMath::DegreesToRadians(currentAngle + AngleBetweenProjectiles / 2)),
			0
		);

		FRotator rotation1 = FRotationMatrix::MakeFromX(p1Dir).Rotator();
		rotation1.Yaw -= 90;
		FRotator rotation2 = FRotationMatrix::MakeFromX(p2Dir).Rotator();
		rotation2.Yaw -= 90;

		AActor* p1 = nullptr; 
		AActor* p2 = nullptr;
		
		p1 = GetWorld()->SpawnActor<AActor>(ProjectileClass, p1Location, rotation1);
		p2 = GetWorld()->SpawnActor<AActor>(ProjectileClass, p2Location, rotation2);

		if (p1->FindComponentByClass<USkeletalMeshComponent>())
		{
			USkeletalMeshComponent* SkelComp1 = p1->FindComponentByClass<USkeletalMeshComponent>();
			USkeletalMeshComponent* SkelComp2 = p2->FindComponentByClass<USkeletalMeshComponent>();

			SkelComp1->SetSimulatePhysics(true);
			SkelComp1->SetEnableGravity(false);
			SkelComp1->SetAllPhysicsLinearVelocity(p1Dir * ProjectileVelocity);

			SkelComp2->SetSimulatePhysics(true);
			SkelComp2->SetEnableGravity(false);
			SkelComp2->SetAllPhysicsLinearVelocity(p2Dir * ProjectileVelocity);
		}
		else
		{
			UPrimitiveComponent* primitiveComp = p1->FindComponentByClass<UPrimitiveComponent>();
			primitiveComp->SetAngularDamping(0);
			primitiveComp->SetLinearDamping(0);
			primitiveComp->SetEnableGravity(false);
			primitiveComp->SetPhysicsLinearVelocity(p1Dir * ProjectileVelocity);

			UPrimitiveComponent* primitiveComp2 = p2->FindComponentByClass<UPrimitiveComponent>();
			primitiveComp2->SetAngularDamping(0);
			primitiveComp2->SetLinearDamping(0);
			primitiveComp2->SetEnableGravity(false);
			primitiveComp2->SetPhysicsLinearVelocity(p2Dir * ProjectileVelocity);
		}			

		currentAngle += angleDelta;
	}
}

bool UGP3_FastProjectileAttack::FailCheck()
{
	if (ProjectileClass == nullptr)		
		return false;

	return true; 
}