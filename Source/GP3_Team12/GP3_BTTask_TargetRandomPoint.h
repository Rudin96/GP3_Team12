// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3_BTTask_TargetRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM12_API UGP3_BTTask_TargetRandomPoint : public UBTTaskNode
{
	GENERATED_BODY()

	UGP3_BTTask_TargetRandomPoint();

public:

	UPROPERTY(EditAnywhere)
		TArray<FVector> RandomPoints;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
