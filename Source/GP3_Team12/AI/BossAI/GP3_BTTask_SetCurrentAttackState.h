// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3_Team12/Enemy/GP3_Boss_AttackBase.h"
#include "GP3_BTTask_SetCurrentAttackState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM12_API UGP3_BTTask_SetCurrentAttackState : public UBTTaskNode
{
	GENERATED_BODY()


protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = "Attack_Type")
		TEnumAsByte<enum AttackType> CurrentAttack = None;
	
};
