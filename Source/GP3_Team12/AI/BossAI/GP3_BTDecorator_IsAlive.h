// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GP3_BTDecorator_IsAlive.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM12_API UGP3_BTDecorator_IsAlive : public UBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
