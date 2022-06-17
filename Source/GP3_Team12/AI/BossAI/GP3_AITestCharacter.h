// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP3_Boss_AIController.h"
#include "GameFramework/Character.h"
#include "GP3_AICharacterBase.h"
#include "GP3_AITestCharacter.generated.h"


UCLASS()
class GP3_TEAM12_API AAI_TestCharacter : public AGP3_AICharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_TestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
