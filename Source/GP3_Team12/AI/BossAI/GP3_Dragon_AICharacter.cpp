// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_Dragon_AICharacter.h"

AGP3_Dragon_AICharacter::AGP3_Dragon_AICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGP3_Dragon_AICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGP3_Dragon_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGP3_Dragon_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
