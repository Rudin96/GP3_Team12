// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GP3_Boss_AIController.generated.h"

class AGP3_CharacterBase;
class AGP3_FirstPlayableCharacter;
class UBlackboardData;

UCLASS()
class GP3_TEAM12_API ABossAIController : public AAIController
{
	GENERATED_BODY()

	ABossAIController();
	void AddAllBossArea();

public:
	const TArray<AActor*> GetAllBossArea() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBlackboardComponent* BlackboardComp;

	UFUNCTION(BlueprintCallable)
		void Init();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
		AGP3_CharacterBase* PlayerCharacter;

	UPROPERTY()
		TArray<AActor*> AllBossArea;

	FName BossAreaTag = "BossArea";
};
