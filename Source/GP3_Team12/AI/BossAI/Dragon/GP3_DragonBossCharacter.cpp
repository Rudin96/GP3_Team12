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

	GetWorldTimerManager().SetTimer(SwitcherHandle, this, &AGP3_DragonBossCharacter::SetSwitchReady, SwitchPlatformDelay, true);
}

AGP3_DragonBossCharacter::AGP3_DragonBossCharacter()
{
	/*Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider Component"));
	RootComponent = Collider;*/
}

void AGP3_DragonBossCharacter::CheckDistances(float DeltaTime)
{
	if (TargetPlatforms.IsEmpty())
		return;

	for (size_t i = 0; i < TargetPlatforms.Num(); i++)
	{
		if (i == TargetActorIndex || i == PreviousTargetIndex)
			continue;

		FVector DistanceToTarget = GetActorLocation() - TargetPlatforms[i]->GetActorLocation();

		FVector NewAttachDir = TargetPlatforms[i]->GetActorLocation() - TargetPlatforms[i]->AttachPosition;

		FVector AttachDirNormal = NewAttachDir.GetSafeNormal();

		float DirDot = FVector::DotProduct(AttachDirNormal, GetActorLocation());

		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Cyan, FString::SanitizeFloat(DirDot));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::SanitizeFloat(DistanceToTarget.Length()));

		DrawDebugSphere(GetWorld(), TargetPlatforms[i]->GetActorLocation(), 600.f, 20, FColor::Yellow);

		if (DistanceToTarget.Length() < TargetActorSwitchDistanceThreshold && bIsSwitchReady)
		{
			PreviousTargetIndex = TargetActorIndex;
			TargetActorIndex = i;
			
			/*if (DirDot < 0.f)
				TargetPlatforms[i]->FlipRotation();*/

			TargetPlatforms[i]->AdjustRotationTowards(DistanceToTarget);
			
			/*if (bSwitchDirOnNewActor)
				SwitchDirection();*/

			/*if (bSwitchRotOnNewActor)
				SwitchRotation();*/

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
	//RotationSpeed = RotationSpeed * -1.f;
}

void AGP3_DragonBossCharacter::SwitchRotation()
{
	//NewRotation = NewRotation.Inverse();
}

void AGP3_DragonBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPlatforms.IsEmpty())
		return;

	TargetActor = TargetPlatforms[TargetActorIndex];

	CurrentLocation = GetActorLocation();

	DrawDebugSphere(GetWorld(), TargetActor->GetActorLocation(), 600.f, 20, FColor::White);
	
	//NewRotation = FQuat::MakeFromEuler(FVector(GetActorRightVector() * RotationSpeed * DeltaTime));

	if(!bIsPendingKill)
		SetActorLocation(FMath::Lerp<FVector, float>(GetActorLocation(), TargetActor->AttachPosition, LerpSpeed * DeltaTime));

	AGP3_CharacterBase* Player = UGP3_GameInstance::GetPlayer(GetWorld());

	if (Player != nullptr)
	{
		FVector Dir = Player->GetActorLocation() - GetActorLocation();
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

	}
		
	CheckDistances(DeltaTime);
}
