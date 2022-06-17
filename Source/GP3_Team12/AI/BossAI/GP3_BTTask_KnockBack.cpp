// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTTask_KnockBack.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"
#include "AIController.h"

UGP3_BTTask_KnockBack::UGP3_BTTask_KnockBack()
{

}

EBTNodeResult::Type UGP3_BTTask_KnockBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGP3_CharacterBase* PlayerCharacter = UGP3_GameInstance::GetPlayer(OwnerComp.GetWorld());

	if (PlayerCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	PlayerCharacter->TakeDamage(20.f, FDamageEvent(), Cast<AController>(OwnerComp.GetAIOwner()), OwnerComp.GetAIOwner()->GetPawn());

	FVector Direction = PlayerCharacter->GetActorLocation() - OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	Direction.Normalize();

	PlayerCharacter->AddImpulse(Direction + (FVector::UpVector * 20.f) * 600);

	return EBTNodeResult::Succeeded;
}