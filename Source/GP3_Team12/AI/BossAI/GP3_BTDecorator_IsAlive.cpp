// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTDecorator_IsAlive.h"
#include "AIController.h"
#include "GP3_AICharacterBase.h"
#include "GP3_Boss_AICharacter.h"
#include "Dragon/GP3_DragonBossCharacter.h"

bool UGP3_BTDecorator_IsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto children = OwnerComp.GetAIOwner()->Children;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>())
		{
			auto bossCharacter = Cast<ABossAICharacter>(children[i]);
			if (bossCharacter != nullptr) 
			{
				if (bossCharacter->bIsPendingKill || bossCharacter->Health <= 0.1f)
				{
					return false;
				}
			}
			break;
		}
	}
	return true;
}
