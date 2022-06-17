// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3_Team12/Enemy/GP3_Boss_AttackBase.h"
#include "GP3_BTTask_Attack.generated.h"

/**
 * + */
UCLASS()
class GP3_TEAM12_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTask_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGP3_BossAttackBase> AttackClass;
};
