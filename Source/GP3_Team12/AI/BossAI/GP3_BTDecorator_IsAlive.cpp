// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTDecorator_IsAlive.h"
#include "AIController.h"
#include "GP3_AICharacterBase.h"

bool UGP3_BTDecorator_IsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto children = OwnerComp.GetAIOwner()->Children;
	AGP3_AICharacterBase* character = nullptr;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>())
		{
			character = Cast<AGP3_AICharacterBase>(children[i]);
			if (character->Health <= 0) 
			{
				return false;
			}
			break;
		}
	}
	return true;
}
