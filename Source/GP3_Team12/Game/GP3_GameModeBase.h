// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GP3_GameModeBase.generated.h"


UCLASS()
class GP3_TEAM12_API AGP3_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Game | Win")
	void WinGame();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWinGame();
};
