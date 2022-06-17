// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTTask_SetMovementSpeed.h"
#include "GP3_Boss_AIController.h"
#include "GP3_AICharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


EBTNodeResult::Type UGP3_BTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto children = OwnerComp.GetAIOwner()->Children;
	AGP3_AICharacterBase* character = nullptr;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>())
		{
			character = Cast<AGP3_AICharacterBase>(children[i]);
			character->GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

			if (MovementSpeed <= 0.01f) 
			{
				character->GetCharacterMovement()->bOrientRotationToMovement = false;
				character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			}
			else 
			{
				character->GetCharacterMovement()->bOrientRotationToMovement = true;
				character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
			}
			break;
		}
	}
	return EBTNodeResult::Succeeded;
}
