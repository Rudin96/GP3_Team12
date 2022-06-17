// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"
#include "GP3_GameInstance.generated.h"

/**
 * 
 */

class UGP3_WhatIfBase;

UCLASS()
class GP3_TEAM12_API UGP3_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	float PlayerHealth = 100.f;

	float PlayerMaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BossBaseHealth = 450.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float PowerOfStage = 4.f;

	TArray<FString> WhatifNames;

	UFUNCTION(BlueprintPure, Meta = (DisplayName = "Get GP3 Game Instance", WorldContext = "WorldContext"), Category = "GP3 | Game Instance")
	static UGP3_GameInstance* Get(UObject* WorldContext);

	virtual void Init() override;

	void ApplyWhatIfs();

	UPROPERTY(BlueprintReadOnly, Category = "Game | Stage")
	int Stage = 1;

	UFUNCTION(BlueprintCallable, Category = "Boss | Health")
	float CalculateBossHealthFromStage();

	UFUNCTION(BlueprintCallable, Category = "Game | Stage")
	void SetStage(int NewStage);

	UFUNCTION(BlueprintCallable, Category = "Game | Stage")
	void IncrementStage();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Player | Stats")
		float DefaultPlayerHealth = 100.f;

	UFUNCTION(BlueprintPure, Meta = (DisplayName = "Get GP3 Player", WorldContext = "WorldContext"), Category = "GP3 | Player")
		static AGP3_CharacterBase* GetPlayer(UObject* WorldContext);
};
