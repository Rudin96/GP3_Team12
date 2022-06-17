// Fill out your copyright notice in the Description page of Project Settings.

#include "GP3_Boss_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GP3_AICharacterBase.h"
#include "GP3_Team12/Game/GP3_GameInstance.h"

#include "GP3_Team12/Character/GP3_CharacterBase.h"

ABossAIController::ABossAIController() 
{
}

void ABossAIController::AddAllBossArea()
{
	TArray<AActor*> allBossArea;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), BossAreaTag, allBossArea);
	
	for (int i = 0; i < allBossArea.Num(); i++)
	{
		AllBossArea.Add(allBossArea[i]);
	}
}

const TArray<AActor*> ABossAIController::GetAllBossArea() const
{
	return AllBossArea;
}

void ABossAIController::Init()
{
	BlackboardComp = GetBlackboardComponent();
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = UGP3_GameInstance::GetPlayer(GetWorld());
	if (PlayerCharacter != nullptr)
	{
		//this->SetFocus(PlayerCharacter);
		BlackboardComp->SetValueAsObject("PlayerCharacter", PlayerCharacter);
	}

	AddAllBossArea();
}

