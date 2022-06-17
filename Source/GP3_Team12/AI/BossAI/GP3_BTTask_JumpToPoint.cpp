// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTTask_JumpToPoint.h"
#include "GP3_Boss_AIController.h"
#include "GP3_AICharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UGP3_BTTask_JumpToPoint::UGP3_BTTask_JumpToPoint() {}

EBTNodeResult::Type UGP3_BTTask_JumpToPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto children = OwnerComp.GetAIOwner()->Children;
	AGP3_AICharacterBase* character = nullptr;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>())
		{
			character = Cast<AGP3_AICharacterBase>(children[i]);
			break;
		}
	}
	if (character != nullptr) 
	{
		FVector currentLocation = character->GetActorLocation();
		FVector targetLocation = character->TargetPoint;
		FVector direction = targetLocation - currentLocation;
		float distance = direction.Length();
		direction.Normalize();
		FVector deltaLocation = (direction * 1000.f) * GetWorld()->GetDeltaSeconds();
		
		character->AddActorLocalOffset(deltaLocation);
	}
	return EBTNodeResult::Succeeded;
}
