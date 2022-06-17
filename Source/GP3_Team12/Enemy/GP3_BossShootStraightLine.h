// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP3_Boss_AttackBase.h"
#include "GP3_BossShootStraightLine.generated.h"

class UGP3_BossAttackFunctions;
class AGP3_CharacterBase;

UCLASS()
class GP3_TEAM12_API UGP3_BossFireStraightLine : public UGP3_BossAttackBase
{
	GENERATED_BODY()

		UGP3_BossFireStraightLine();

public:
	UFUNCTION()
		void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void Attack_Implementation() override;

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
		TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
		FVector SpawnOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
		float ProjectileSpeed = 2500.f;

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
		float TargetDirectionOffset = 0.5f;

private:
	UGP3_BossAttackFunctions* BossAttackFunctions;
	AGP3_CharacterBase* PlayerRef = nullptr;
};
