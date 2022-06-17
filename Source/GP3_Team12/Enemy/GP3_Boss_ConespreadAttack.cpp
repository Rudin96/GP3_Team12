#include "GP3_Boss_ConespreadAttack.h"
#include "GP3_Boss_AttackFunctions.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

UGP3_BossConespreadAttack::UGP3_BossConespreadAttack()
{
}

void UGP3_BossConespreadAttack::BeginPlay()
{
	Super::BeginPlay(); 

	BossAttackFunctions = GetOwner()->FindComponentByClass<UGP3_BossAttackFunctions>();

	//If BossAttackFunctions (UActorComponent) isn't found.. Add an instance of it
	if (BossAttackFunctions == nullptr)
		BossAttackFunctions = Cast<UGP3_BossAttackFunctions>(GetOwner()->AddComponentByClass(UGP3_BossAttackFunctions::StaticClass(), true, FTransform(), true));

	AActor* owner = GetOwner(); 
	ConeStartLocation = BossAttackFunctions->GetSceneComponentThroughTag(owner, FName(TEXT("ConespreadAttack")));
}

void UGP3_BossConespreadAttack::Attack_Implementation()
{
	if (!FailCheck())
		return; 

	float currentAngle = StartAngle; //current angle for around boss for each cone	
	float currentRelativeAngle = currentAngle; //current angle relative from start of current cone
	float currentRadiusFromBoss = RadiusFromBoss; //current distance from boss

	FVector currentLocation; 
	if (ConeStartLocation != nullptr)
		currentLocation = ConeStartLocation->GetComponentLocation(); 
	else
		currentLocation = GetOwner()->GetActorLocation();
	
	currentLocation += SpawnOffset;

	for (size_t i = 0; i < Rings; i++)
	{
		for (size_t y = 0; y < ConeAmount; y++)
		{
			//Get Locations
			FVector p1Location = FVector(
				currentLocation.X + currentRadiusFromBoss * cos(FMath::DegreesToRadians(currentAngle)),
				currentLocation.Y + currentRadiusFromBoss * sin(FMath::DegreesToRadians(currentAngle)),
				currentLocation.Z);			

			//Get Direction for velocity application
			FVector dir1 = FVector(
			currentRadiusFromBoss * cos(FMath::DegreesToRadians(currentAngle)),
			currentRadiusFromBoss * sin(FMath::DegreesToRadians(currentAngle)),
			0);
			dir1.Normalize(); 

			currentRelativeAngle = currentAngle - AngleIncreasement;

			FVector p2Location = FVector(
				currentLocation.X + currentRadiusFromBoss * cos(FMath::DegreesToRadians(currentRelativeAngle)),
				currentLocation.Y + currentRadiusFromBoss * sin(FMath::DegreesToRadians(currentRelativeAngle)),
				currentLocation.Z);

			FVector dir2 = FVector(
				currentRadiusFromBoss * cos(FMath::DegreesToRadians(currentRelativeAngle)),
				currentRadiusFromBoss * sin(FMath::DegreesToRadians(currentRelativeAngle)),
				0);
			dir2.Normalize(); 

			currentRelativeAngle = currentAngle + AngleIncreasement;

			FVector p3Location = FVector(
				currentLocation.X + currentRadiusFromBoss * cos(FMath::DegreesToRadians(currentRelativeAngle)),
				currentLocation.Y + currentRadiusFromBoss * sin(FMath::DegreesToRadians(currentRelativeAngle)),
				currentLocation.Z);

			FVector dir3 = FVector(
				currentRadiusFromBoss * cos(FMath::DegreesToRadians(currentRelativeAngle)),
				currentRadiusFromBoss * sin(FMath::DegreesToRadians(currentRelativeAngle)),
				0);
			dir3.Normalize(); 

			//Spawn actors and add velocity			
			FRotator rotation1 = FRotationMatrix::MakeFromX(dir1).Rotator(); 
			rotation1.Yaw -= 90; 
			FRotator rotation2 = FRotationMatrix::MakeFromX(dir2).Rotator(); 
			rotation2.Yaw -= 90;
			FRotator rotation3 = FRotationMatrix::MakeFromX(dir3).Rotator(); 
			rotation3.Yaw -= 90;

			AActor* p1 = GetWorld()->SpawnActor<AActor>(ProjectileClass, p1Location, rotation1);
			AActor* p2 = GetWorld()->SpawnActor<AActor>(ProjectileClass, p2Location, rotation2);
			AActor* p3 = GetWorld()->SpawnActor<AActor>(ProjectileClass, p3Location, rotation3);			

			if (p1 == nullptr || p2 == nullptr || p3 == nullptr)
				return;

			if (p1->FindComponentByClass<USkeletalMeshComponent>())
			{
				USkeletalMeshComponent* SkelComp1 = p1->FindComponentByClass<USkeletalMeshComponent>();
				USkeletalMeshComponent* SkelComp2 = p2->FindComponentByClass<USkeletalMeshComponent>();
				USkeletalMeshComponent* SkelComp3 = p3->FindComponentByClass<USkeletalMeshComponent>();

				SkelComp1->SetSimulatePhysics(true);
				SkelComp1->SetEnableGravity(false);
				SkelComp1->SetAllPhysicsLinearVelocity(dir1 * ProjectileVelocity);

				SkelComp2->SetSimulatePhysics(true);
				SkelComp2->SetEnableGravity(false);
				SkelComp2->SetAllPhysicsLinearVelocity(dir2 * ProjectileVelocity);

				SkelComp3->SetSimulatePhysics(true);
				SkelComp3->SetEnableGravity(false);
				SkelComp3->SetAllPhysicsLinearVelocity(dir3 * ProjectileVelocity);
			}
			else
			{
				UPrimitiveComponent* primitiveComp1 = p1->FindComponentByClass<UPrimitiveComponent>();
				primitiveComp1->SetAngularDamping(0);
				primitiveComp1->SetLinearDamping(0); //Set Dampings to 0
				primitiveComp1->SetEnableGravity(false); //Disable gravity for projectile
				primitiveComp1->SetPhysicsLinearVelocity(dir1 * ProjectileVelocity); //Set velocity

				UPrimitiveComponent* primitiveComp2 = p2->FindComponentByClass<UPrimitiveComponent>();
				primitiveComp2->SetAngularDamping(0);
				primitiveComp2->SetLinearDamping(0);
				primitiveComp2->SetEnableGravity(false);
				primitiveComp2->SetPhysicsLinearVelocity(dir2 * ProjectileVelocity);

				UPrimitiveComponent* primitiveComp3 = p3->FindComponentByClass<UPrimitiveComponent>();
				primitiveComp3->SetAngularDamping(0);
				primitiveComp3->SetLinearDamping(0);
				primitiveComp3->SetEnableGravity(false);
				primitiveComp3->SetPhysicsLinearVelocity(dir3 * ProjectileVelocity);
			}				

			currentAngle += 360 / ConeAmount; //Go to startposition for next "cone"
		}

		currentRadiusFromBoss += 400; //Add a bit of offset from the previous ring of projectiles
	}
}

bool UGP3_BossConespreadAttack::FailCheck()
{
	if (ProjectileClass == nullptr)		
		return false;

	return true; 
}