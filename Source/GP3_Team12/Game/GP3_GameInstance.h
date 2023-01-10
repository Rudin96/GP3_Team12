// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"
#include "GP3_Team12/AI/BossAI/GP3_Boss_AICharacter.h"
#include "GP3_GameInstance.generated.h"

/**
 * 
 */

class UGP3_WhatIfBase;

UCLASS(Config=Game)
class GP3_TEAM12_API UGP3_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	FTimerHandle AudioInitTimerHandle;

private:
	
	UFUNCTION()
	void InitAudio() const;

public:
	
	float PlayerHealth = 100.f;

	float PlayerMaxHealth = 100.f;

	ABossAICharacter* CurrentBoss;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BossBaseHealth = 450.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float PowerOfStage = 4.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config)
	float MasterVolume = 1.f;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USoundMix* MasterMix;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config)
	bool bShowStats;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config)
	float CameraSensitivity = 1.f;

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

	UFUNCTION(BlueprintCallable)
	void SetCameraSensitivity(float Sensitivity);
	
	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float Volume);
	
	UFUNCTION(BlueprintCallable)
	void SetShowStats(bool Show);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Player | Stats")
		float DefaultPlayerHealth = 100.f;

	UFUNCTION(BlueprintPure, Meta = (DisplayName = "Get GP3 Player", WorldContext = "WorldContext"), Category = "GP3 | Player")
		static AGP3_CharacterBase* GetPlayer(UObject* WorldContext);
};
