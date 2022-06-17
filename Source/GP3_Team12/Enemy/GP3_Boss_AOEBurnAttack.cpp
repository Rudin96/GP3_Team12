#include "GP3_Boss_AOEBurnAttack.h"
#include "GP3_Boss_AttackFunctions.h"
#include "Math/UnrealMathUtility.h"

UGP3_BossAOEBurnAttack::UGP3_BossAOEBurnAttack()
{
}

void UGP3_BossAOEBurnAttack::BeginPlay()
{
	Super::BeginPlay();
	BossAttackFunctions = GetOwner()->FindComponentByClass<UGP3_BossAttackFunctions>();	

	//If BossAttackFunctions (UActorComponent) isn't found.. Add an instance of it to owning actor
	if(BossAttackFunctions == nullptr)
		BossAttackFunctions = Cast<UGP3_BossAttackFunctions>(GetOwner()->AddComponentByClass(UGP3_BossAttackFunctions::StaticClass(), true, FTransform(), true));
}

void UGP3_BossAOEBurnAttack::Attack_Implementation()
{
	if (!FailCheck())
		return; 

	float radius = FMath::RandRange(MaxRadius / 2, MaxRadius);
	float angle = FMath::RandRange(0, 360);
	FVector currentPos = GetOwner()->GetActorLocation();

	//Create random target position from angle and radius
	FVector targetPos = FVector(
		currentPos.X + radius * cos(FMath::DegreesToRadians(angle)),
		currentPos.Y + radius * sin(FMath::DegreesToRadians(angle)),
		currentPos.Z
	);
	
	if (!BossAttackFunctions->GroundCheck(targetPos, FVector(targetPos.X, targetPos.Y, -1000)))
	{
		Iteration++;

		if (Iteration >= MaxIteration)
			return;
		else
		{
			Attack_Implementation();
			return;
		}
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(AOEActorClass, targetPos, FRotator::ZeroRotator);
	}
}

bool UGP3_BossAOEBurnAttack::FailCheck()
{
	if (BossAttackFunctions == nullptr)
		return false;
	if (AOEActorClass == nullptr)
		return false;
	
	return true; 
}