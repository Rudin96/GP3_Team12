#pragma once

#include "Components/ActorComponent.h"

#include "GP3_Boss_AttackFunctions.generated.h"

UCLASS(Blueprintable)
class UGP3_BossAttackFunctions : public UActorComponent{

	GENERATED_BODY()

public: 

	UGP3_BossAttackFunctions();

	bool GroundCheck(FVector StartPos, FVector TargetPos);

	FVector CalculateParabolaVelocity(FVector StartPos, FVector Target, float Amplitude);

	USceneComponent* GetSceneComponentThroughTag(AActor*& Actor, FName Tag);
};