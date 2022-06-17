// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP3_Team12/AI/BossAI/GP3_AICharacterBase.h"
#include "../GP3_Boss_AICharacter.h"
#include "GP3_DragonBossCharacter.generated.h"

/**
 * 
 */

class USphereComponent;

UCLASS()
class GP3_TEAM12_API AGP3_DragonBossCharacter : public ABossAICharacter
{
	GENERATED_BODY()

	AGP3_DragonBossCharacter();

public:
	UFUNCTION(BlueprintCallable)
		void SwitchDirection();

	UFUNCTION(BlueprintCallable)
		void SwitchRotation();

protected: 

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:

	FQuat NewRotation;

	FVector NewPosition;

	FVector CurrentLocation;

	AActor* TargetActor;

	FTimerHandle SwitcherHandle;

	int PreviousTargetIndex = 0;

	void CheckDistances(float DeltaTime);

	void SetSwitchReady();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USphereComponent* Collider;

	UPROPERTY(EditAnywhere)
		FVector LocOffset;

	UPROPERTY(EditAnywhere)
		FRotator RotOffset;

		float MaxDistance = 1000.f;

		float DistanceOffset = 1000.f;

	UPROPERTY(EditAnywhere)
		float RotationSpeed = 30.f;

	UPROPERTY(EditAnywhere)
		int TargetActorIndex = 0;

	UPROPERTY(EditAnywhere)
		float SwitchPlatformDelay = 10.f;

	UPROPERTY(EditAnywhere)
		bool bSwitchDirOnNewActor = true;

	UPROPERTY(EditAnywhere)
		bool bSwitchRotOnNewActor = true;

	UPROPERTY(EditAnywhere)
		bool bIsSwitchReady = true;

	UPROPERTY(EditAnywhere)
		bool bRotateTowardsTarget = true;

	UPROPERTY(EditAnywhere)
		float TargetActorSwitchDistanceThreshold = 500.f;

	UPROPERTY(EditAnywhere)
		float LerpSpeed = 2.f;

	UPROPERTY(EditAnywhere)
		float LerpSpeedRotation = 2.f;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> TargetActors;
};
