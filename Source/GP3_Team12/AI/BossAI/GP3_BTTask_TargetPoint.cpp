// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTTask_TargetPoint.h"
#include "GP3_Boss_AIController.h"
#include "GP3_AICharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UGP3_BTTask_TargetPoint::UGP3_BTTask_TargetPoint() {}

EBTNodeResult::Type UGP3_BTTask_TargetPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto children = OwnerComp.GetAIOwner()->Children;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>()) 
		{
			AGP3_AICharacterBase* character = Cast<AGP3_AICharacterBase>(children[i]);
			TArray patrolPoints = character->PatrolPoints;
			const int arraySize = patrolPoints.Num();
			int randomNumber = rand() % arraySize;

			UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
			FVector currentTargetPoint = blackboardComponent->GetValueAsVector("PatrolPoint");

			while (currentTargetPoint == patrolPoints[randomNumber]) 
			{
				randomNumber = rand() % arraySize;
			}
			blackboardComponent->SetValueAsVector("PatrolPoint", patrolPoints[randomNumber]);
			character->TargetPoint = patrolPoints[randomNumber];
			break;
		}
	}
	return EBTNodeResult::Succeeded;
}
