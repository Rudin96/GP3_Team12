// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_GameInstance.h"

#include "AudioDevice.h"
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
	LoadConfig();

	UE_LOG(LogTemp, Log, TEXT("UGP3_GameInstance called!"));

	GetWorld()->GetTimerManager().SetTimer(AudioInitTimerHandle,this, &UGP3_GameInstance::InitAudio, 0.1f,false);
}



void UGP3_GameInstance::ApplyWhatIfs()
{
	if (AGP3_WhatIfBrain* FoundBrain = Cast<AGP3_WhatIfBrain>(UGameplayStatics::GetActorOfClass(GetWorld(), AGP3_WhatIfBrain::StaticClass())))
	{
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

	Stage = NewStage;

	if (NewStage == 1)
	{
		WhatifNames.Empty();
	}

	if (NewStage > 1)
	{

		AGP3_CharacterBase* Player = GetPlayer(GetWorld());
		
		if (Player == nullptr)
			return;

		PlayerHealth = Player->Health;
	}
}

void UGP3_GameInstance::IncrementStage()
{
	SetStage(++Stage);
}

void UGP3_GameInstance::SetCameraSensitivity(const float Sensitivity)
{
	CameraSensitivity = Sensitivity;
	SaveConfig();
	AGP3_CharacterBase* Player = GetPlayer(GetWorld());
	if (Player != nullptr)
	{
		Player->SetCameraSensitivity(Sensitivity);
	}
}

void UGP3_GameInstance::SetMasterVolume(const float Volume)
{
	MasterVolume = Volume;
	SaveConfig();
}

void UGP3_GameInstance::SetShowStats(const bool Show)
{
	bShowStats = Show;
	SaveConfig();
}

void UGP3_GameInstance::InitAudio() const
{
	GetWorld()->GetAudioDevice().GetAudioDevice()->SetSoundMixClassOverride(MasterMix,nullptr, MasterVolume,1.f,0.1f,true);
	GetWorld()->GetAudioDevice().GetAudioDevice()->SetBaseSoundMix(MasterMix); //PushSoundMixModifier(MasterMix);
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
