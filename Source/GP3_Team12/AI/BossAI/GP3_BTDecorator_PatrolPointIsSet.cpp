// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTDecorator_PatrolPointIsSet.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UGP3_BTDecorator_PatrolPointIsSet::UGP3_BTDecorator_PatrolPointIsSet() {}

bool UGP3_BTDecorator_PatrolPointIsSet::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FVector Location = OwnerComp.GetBlackboardComponent()->GetValueAsVector("PatrolPoint");
	if (Location != FAISystem::InvalidLocation)
	{
		return true;
	}
	return false;
}
