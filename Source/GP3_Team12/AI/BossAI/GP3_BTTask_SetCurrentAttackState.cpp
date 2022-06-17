// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTTask_SetCurrentAttackState.h"
#include "AIController.h"
#include "GP3_AICharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

EBTNodeResult::Type UGP3_BTTask_SetCurrentAttackState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	auto children = controller->Children;
	AGP3_AICharacterBase* character = nullptr;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>())
		{
			character = Cast<AGP3_AICharacterBase>(children[i]);
			character->CurrentAttack = CurrentAttack;
			if (CurrentAttack == AttackType::None) 
			{
				controller->ClearFocus(EAIFocusPriority::Gameplay);
			}
			else
			{
				AActor* player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerCharacter"));
				controller->SetFocus(player);
			}
			break;
		}
	}
	return EBTNodeResult::Succeeded;
}
