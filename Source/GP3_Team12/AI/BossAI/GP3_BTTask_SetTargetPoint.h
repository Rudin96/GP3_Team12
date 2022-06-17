// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3_BTTask_SetTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM12_API UGP3_BTTask_SetTargetPoint : public UBTTaskNode
{
	GENERATED_BODY()
		UGP3_BTTask_SetTargetPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
