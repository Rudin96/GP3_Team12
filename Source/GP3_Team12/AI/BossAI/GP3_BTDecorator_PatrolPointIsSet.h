// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GP3_BTDecorator_PatrolPointIsSet.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM12_API UGP3_BTDecorator_PatrolPointIsSet : public UBTDecorator
{
	GENERATED_BODY()

		UGP3_BTDecorator_PatrolPointIsSet();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
