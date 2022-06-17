// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3_Team12/Game/GP3_GameInstance.h"
#include "GP3_BTTask_KnockBack.generated.h"

class AGP3_CharacterBase;

UCLASS()
class GP3_TEAM12_API UGP3_BTTask_KnockBack : public UBTTaskNode
{
	GENERATED_BODY()
		UGP3_BTTask_KnockBack();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	UPROPERTY(EditAnywhere)
		float KnockbackForce = 600.f;

};
