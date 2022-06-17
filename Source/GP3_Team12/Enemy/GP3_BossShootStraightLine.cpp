// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BossShootStraightLine.h"
#include "GP3_Boss_AttackFunctions.h"
#include <GP3_Team12/Game/GP3_GameInstance.h>

#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

UGP3_BossFireStraightLine::UGP3_BossFireStraightLine() 
{
}

void UGP3_BossFireStraightLine::BeginPlay()
{
	Super::BeginPlay();

	BossAttackFunctions = GetOwner()->FindComponentByClass<UGP3_BossAttackFunctions>();

	if (BossAttackFunctions == nullptr)
	{
		BossAttackFunctions = Cast<UGP3_BossAttackFunctions>(GetOwner()->AddComponentByClass(
			UGP3_BossAttackFunctions::StaticClass(), true, FTransform(), true));
	}
	PlayerRef = UGP3_GameInstance::GetPlayer(GetWorld());
}

void UGP3_BossFireStraightLine::Attack_Implementation()
{
	if (PlayerRef == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, TEXT("PlayerRef == nullptr - GP3_Boss_ShootStraightLine.cpp"));
		return;
	}
	FVector currentLocation = GetOwner()->GetActorLocation() + SpawnOffset;
	FVector playerVelocity = PlayerRef->GetPlayerVelocity();
	playerVelocity.Z = 0.f;
	FVector targetLocation = PlayerRef->GetActorLocation() + (playerVelocity * TargetDirectionOffset);

	AActor* projectile = nullptr;
	projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, currentLocation, FRotator::ZeroRotator);
	if (projectile != nullptr)
	{
		FVector projectileVelocity = targetLocation - currentLocation;
		projectileVelocity.Normalize();
		projectileVelocity.Z = 0.f;

		USkeletalMeshComponent* SkelComp1 = projectile->FindComponentByClass<USkeletalMeshComponent>();
		SkelComp1->SetEnableGravity(false);
		SkelComp1->SetSimulatePhysics(true);
		SkelComp1->SetAllPhysicsLinearVelocity(projectileVelocity * ProjectileSpeed);
	}
}
