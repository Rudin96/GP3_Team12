#include "GP3_Boss_LaunchAttack.h"
#include "GP3_Boss_AttackFunctions.h"

#include "Components/PrimitiveComponent.h"

#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"

UGP3_BossLaunchAttack::UGP3_BossLaunchAttack()
{

}

void UGP3_BossLaunchAttack::BeginPlay()
{
	Super::BeginPlay();

	BossAttackFunctions = GetOwner()->FindComponentByClass<UGP3_BossAttackFunctions>();

	//If BossAttackFunctions (UActorComponent) isn't found.. Add an instance of it
	if (BossAttackFunctions == nullptr)
		BossAttackFunctions = Cast<UGP3_BossAttackFunctions>(GetOwner()->AddComponentByClass(UGP3_BossAttackFunctions::StaticClass(), true, FTransform(), true));
}

void UGP3_BossLaunchAttack::Attack_Implementation()
{
	if (!FailCheck())
		return; 
	
	float radius = FMath::RandRange(MaxRadius / 2, MaxRadius);
	int angle = FMath::RandRange(0, 360);

	FVector currentPos = GetOwner()->GetActorLocation();

	//Create random target position from angle and radius
	FVector targetPos = FVector(
		currentPos.X + radius * cos(FMath::DegreesToRadians(angle)),
		currentPos.Y + radius * sin(FMath::DegreesToRadians(angle)),
		currentPos.Z
	);

	//Check that target point is above/on ground (if not, run function again)
	if (!BossAttackFunctions->GroundCheck(FVector(targetPos.X, targetPos.Y, 0), FVector(targetPos.X, targetPos.Y, -200)))
	{
		Iteration++;
		if (Iteration > MaxIteration)					
			return;
		
		Attack_Implementation();
	}
	else
	{
		//Spawn visual cue where boss will land (if DecalClass has been set)
		if (DecalClass != nullptr)
		{
			AActor* targetDecal = nullptr;
			FVector pos = FVector(targetPos.X, targetPos.Y, 0);
			targetDecal = GetWorld()->SpawnActor<AActor>(DecalClass, pos, FRotator::ZeroRotator);
		}

		//Get velocity from target location
		FVector velocity = BossAttackFunctions->CalculateParabolaVelocity(currentPos, targetPos, JumpHeight);

		//Get PrimitiveComponent and set velocity
		UPrimitiveComponent* primitiveComp = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
		primitiveComp->SetAngularDamping(0);
		primitiveComp->SetLinearDamping(0);
		primitiveComp->SetPhysicsLinearVelocity(velocity);
	}
}

bool UGP3_BossLaunchAttack::FailCheck()
{
	if (BossAttackFunctions == nullptr)
		return false;

	return true; 
}