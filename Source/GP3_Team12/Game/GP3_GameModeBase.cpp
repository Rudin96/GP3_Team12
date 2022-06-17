// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_GameModeBase.h"
#include "GP3_GameInstance.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"

void AGP3_GameModeBase::WinGame()
{
	UGP3_GameInstance* GameInstance = UGP3_GameInstance::Get(GetWorld());

	AGP3_CharacterBase* Player = UGP3_GameInstance::GetPlayer(GetWorld());

	OnWinGame();
}