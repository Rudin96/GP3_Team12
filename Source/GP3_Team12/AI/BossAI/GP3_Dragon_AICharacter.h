// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP3_AICharacterBase.h"
#include "GP3_Dragon_AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM12_API AGP3_Dragon_AICharacter : public AGP3_AICharacterBase
{
	GENERATED_BODY()

	AGP3_Dragon_AICharacter();
	
public:

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
