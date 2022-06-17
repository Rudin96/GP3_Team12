// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_GameInstance.h"
#include "GP3_Team12/AI/BossAI/GP3_Boss_AICharacter.h"
#include "GP3_Team12/Game/GP3_WhatIf_Base.h"
#include "GP3_Team12/Game/GP3_WhatIf_Brain.h"
#include "Kismet/GameplayStatics.h"

UGP3_GameInstance* UGP3_GameInstance::Get(UObject* WorldContext)
{
	UWorld* World = WorldContext->GetWorld();
	return World->GetGameInstance<UGP3_GameInstance>();
}

void UGP3_GameInstance::Init()
{
	Super::Init();
	//CurrentPlayer = GetPlayer(GetWorld());
	UE_LOG(LogTemp, Log, TEXT("UGP3_GameInstance called!"));
}

void UGP3_GameInstance::ApplyWhatIfs()
{
	if (AGP3_WhatIfBrain* FoundBrain = Cast<AGP3_WhatIfBrain>(UGameplayStatics::GetActorOfClass(GetWorld(), AGP3_WhatIfBrain::StaticClass())))
	{
		//WhatifNames.Empty();
		for (auto s : WhatifNames)
		{
			FoundBrain->ChangeWhatIf(s);
		}
	}
}

float UGP3_GameInstance::CalculateBossHealthFromStage()
{
	return BossBaseHealth + (BossBaseHealth * Stage) + (FMath::Pow(Stage, PowerOfStage));
}

void UGP3_GameInstance::SetStage(int NewStage = 1)
{
	if (NewStage < 1)
		return;

	if (NewStage == 1)
	{
		WhatifNames.Empty();
	}

	if (NewStage > 1)
	{

		AGP3_CharacterBase* Player = GetPlayer(GetWorld());
		
		if (Player == nullptr)
			return;

		PlayerMaxHealth = Player->MaxHealth;
		PlayerHealth = Player->Health;
	}

	/*ABossAICharacter* CurrentBoss = ABossAICharacter::Get(GetWorld());

	float OldPercentageHealth = CurrentBoss->Health / CurrentBoss->MaxHealth;

	CurrentBoss->MaxHealth = CalculateBossHealthFromStage();
	CurrentBoss->Health = CurrentBoss->MaxHealth * OldPercentageHealth;

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.5f, FColor::Black, FString::Printf(TEXT("New health equal to %f"), CurrentBoss->Health));*/
}

void UGP3_GameInstance::IncrementStage()
{
	SetStage(++Stage);
}

AGP3_CharacterBase* UGP3_GameInstance::GetPlayer(UObject* WorldContext)
{
	APlayerController* Controller = WorldContext->GetWorld()->GetFirstPlayerController();
	if (Controller == nullptr)
		return nullptr;

	if (AGP3_CharacterBase* Character = Cast<AGP3_CharacterBase>(Controller->GetPawn()))
	{
		return Character;
	}
	else
	{
		return nullptr;
	}
}
