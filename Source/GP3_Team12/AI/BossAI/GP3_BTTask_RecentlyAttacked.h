// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3_BTTask_RecentlyAttacked.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM12_API UGP3_BTTaskNode_RecentlyAttacked : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


public:
	UPROPERTY(EditAnywhere)
		float RestTime = -1.f;

private:
	bool RestTimer();
	float TimeStamp = 0.f;
};
