// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3_BTTaskNode_WaitForAttack.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM12_API UGP3_BTTaskNode_WaitForAttack : public UBTTaskNode
{
	GENERATED_BODY()

	UGP3_BTTaskNode_WaitForAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


public:
	UPROPERTY(EditAnywhere)
		float AttackCooldown = 1.f;

	UPROPERTY(EditAnywhere)
		float RandomDeviation = 0.5f;

private:
	bool TimeToAttack();
	float RandomFloat(float min, float max);

	float NewAttackCooldown = 1.f;
	float TimeStamp = -1.f;

};
