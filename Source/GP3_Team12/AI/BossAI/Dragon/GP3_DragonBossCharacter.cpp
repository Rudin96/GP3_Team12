// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_Team12/AI/BossAI/Dragon/GP3_DragonBossCharacter.h"
#include "Engine/RendererSettings.h"
#include "Components/SphereComponent.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"
#include "GP3_Team12/Game/GP3_GameInstance.h"
#include <GP3_Team12/Enemy/Dragon_Boss/GP3_DragonBoss_PlatformHandler.h>

void AGP3_DragonBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SwitcherHandle, this, &AGP3_DragonBossCharacter::SetSwitchReady, SwitchPlatformDelay, true, SwitchPlatformDelay);

	auto* RenderSettings = GetMutableDefault<URendererSettings>();

	RenderSettings->MSAASampleCount = ECompositingSampleCount::Eight;
}

AGP3_DragonBossCharacter::AGP3_DragonBossCharacter()
{
	/*Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider Component"));
	RootComponent = Collider;*/
}

void AGP3_DragonBossCharacter::CheckDistances(float DeltaTime)
{
	if (TargetActors.IsEmpty())
		return;

	for (size_t i = 0; i < TargetActors.Num(); i++)
	{
		if (i == TargetActorIndex || i == PreviousTargetIndex)
			continue;

		FVector DistanceToTarget = TargetActors[i]->GetActorLocation() - GetActorLocation();

		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::SanitizeFloat(DistanceToTarget.Length()));

		if (DistanceToTarget.Length() < TargetActorSwitchDistanceThreshold && bIsSwitchReady)
		{
			PreviousTargetIndex = TargetActorIndex;
			TargetActorIndex = i;
			if (bSwitchDirOnNewActor)
				SwitchDirection();
			if (bSwitchRotOnNewActor)
				SwitchRotation();

			bIsSwitchReady = false;
			break;
		}
	}
}

void AGP3_DragonBossCharacter::SetSwitchReady()
{
	bIsSwitchReady = true;
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("SWITCH READY BRUH! :)"));
}

void AGP3_DragonBossCharacter::SwitchDirection()
{
	RotationSpeed = RotationSpeed * -1.f;
}

void AGP3_DragonBossCharacter::SwitchRotation()
{
	NewRotation = NewRotation.Inverse();
}

void AGP3_DragonBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActors.IsEmpty())
		return;

	TargetActor = TargetActors[TargetActorIndex];

	CurrentLocation = GetActorLocation();

	DrawDebugSphere(GetWorld(), TargetActor->GetActorLocation(), 600.f, 20.f, FColor::White);
	
	//NewRotation = FQuat::MakeFromEuler(FVector(GetActorRightVector() * RotationSpeed * DeltaTime));

	NewRotation = FQuat::MakeFromEuler(FVector(NewRotation.Euler().X, NewRotation.Euler().Y, NewRotation.Euler().Z + RotationSpeed * DeltaTime));

	NewPosition = NewRotation * LocOffset + TargetActor->GetActorLocation();

	DrawDebugLine(GetWorld(), TargetActor->GetActorLocation(), NewPosition, FColor::Magenta, false, 0.f, (uint8)0U, 5.f);

	//SetActorLocation(NewPosition);
	SetActorLocation(FMath::Lerp<FVector, float>(GetActorLocation(), NewPosition, LerpSpeed * DeltaTime));

	FVector Dir = UGP3_GameInstance::GetPlayer(GetWorld())->GetActorLocation() - GetActorLocation();
	Dir.Normalize();

	if (bRotateTowardsTarget)
	{
		FRotator TargetRotation = FRotator(Dir.ToOrientationRotator()) + RotOffset;
		SetActorRotation(FMath::Lerp<FRotator, float>(GetActorRotation(), TargetRotation, LerpSpeedRotation * DeltaTime));
	}
	else
	{
		FRotator TargetRotation = RotOffset;
		SetActorRotation(FMath::Lerp<FRotator, float>(GetActorRotation(), TargetRotation, LerpSpeedRotation * DeltaTime));
	}


	CheckDistances(DeltaTime);
}
