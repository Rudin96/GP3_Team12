#include "GP3_Boss_ProjectileRingAttack.h"
#include "GP3_Boss_AttackFunctions.h"

void UGP3_ProjectileRingAttack::BeginPlay()
{
	Super::BeginPlay(); 
	
	BossAttackFunctions = GetOwner()->FindComponentByClass<UGP3_BossAttackFunctions>();

	//If BossAttackFunctions (UActorComponent) isn't found.. Add an instance of it
	if (BossAttackFunctions == nullptr)
		BossAttackFunctions = Cast<UGP3_BossAttackFunctions>(GetOwner()->AddComponentByClass(UGP3_BossAttackFunctions::StaticClass(), true, FTransform(), true));

	AActor* owner = GetOwner(); 
	ProjectileStartLocation = BossAttackFunctions->GetSceneComponentThroughTag(owner, "ProjectileRingAttack");
}

void UGP3_ProjectileRingAttack::Attack_Implementation()
{
	if (!FailCheck())
		return;

	FVector currentLocation; 

	if (ProjectileStartLocation != nullptr)
		currentLocation = ProjectileStartLocation->GetComponentLocation();
	else
		currentLocation = GetOwner()->GetActorLocation(); 

	float angleDelta = 360 / ProjectileAmount; 
	float currentAngle = 0;

	for (size_t i = 0; i < ProjectileAmount; i++)
	{
		FVector targetLocation = FVector(
		currentLocation.X + StartRadius * cos(FMath::DegreesToRadians(currentAngle)),
		currentLocation.Y + StartRadius * sin(FMath::DegreesToRadians(currentAngle)),
		currentLocation.Z
		);

		FVector dir = FVector(
		cos(FMath::DegreesToRadians(currentAngle)),
		sin(FMath::DegreesToRadians(currentAngle)),
		0
		);

		FRotator rotation = FRotationMatrix::MakeFromX(dir).Rotator();
		rotation.Yaw -= 90;

		AActor* projectile = nullptr; 
	
		projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, targetLocation, rotation);

		if (projectile->FindComponentByClass<USkeletalMeshComponent>())
		{
			USkeletalMeshComponent* SkelComp = projectile->FindComponentByClass<USkeletalMeshComponent>();

			SkelComp->SetSimulatePhysics(true);
			SkelComp->SetEnableGravity(false);
			SkelComp->SetAllPhysicsLinearVelocity(dir * ProjectileVelocity);
		}
		else
		{
			UPrimitiveComponent* primitiveComp = projectile->FindComponentByClass<UPrimitiveComponent>();
			primitiveComp->SetAngularDamping(0);
			primitiveComp->SetLinearDamping(0);
			primitiveComp->SetSimulatePhysics(true);
			primitiveComp->SetEnableGravity(false);
			primitiveComp->SetPhysicsLinearVelocity(dir * ProjectileVelocity);
		}	

		currentAngle += angleDelta; 
	}
}

bool UGP3_ProjectileRingAttack::FailCheck()
{
	if (BossAttackFunctions == nullptr)
		return false;
	if (ProjectileClass == nullptr)			
		return false;
	
	return true; 
}