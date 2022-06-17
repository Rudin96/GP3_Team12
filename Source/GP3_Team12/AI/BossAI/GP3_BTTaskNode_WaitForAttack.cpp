// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTTaskNode_WaitForAttack.h"
#include "AIController.h"
#include "GP3_Team12/Game/GP3_GameInstance.h"
#include "GP3_AICharacterBase.h"

UGP3_BTTaskNode_WaitForAttack::UGP3_BTTaskNode_WaitForAttack() 
{
	NewAttackCooldown = AttackCooldown;
}

EBTNodeResult::Type UGP3_BTTaskNode_WaitForAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (TimeToAttack())
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

bool UGP3_BTTaskNode_WaitForAttack::TimeToAttack()
{
	if (GetWorld()->GetTimeSeconds() - TimeStamp >= NewAttackCooldown)
	{
		const float randomValue = RandomFloat(-RandomDeviation, RandomDeviation);	
		UGP3_GameInstance* gameInstance = UGP3_GameInstance::Get(GetWorld());
		int stage = 9 - gameInstance->Stage;
		float crazyKohlerMaff = ((AttackCooldown + (float)stage) / 10) * AttackCooldown;
		NewAttackCooldown = crazyKohlerMaff + randomValue;
		TimeStamp = GetWorld()->GetTimeSeconds();
		return true;
	}
	return false;
}

float UGP3_BTTaskNode_WaitForAttack::RandomFloat(float min, float max)
{
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}
