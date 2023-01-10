// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GP3_Team12/Enemy/GP3_Boss_AttackBase.h"
#include "GP3_AICharacterBase.generated.h"

class UBlackboardData;
class UBehaviorTree;

UCLASS()
class GP3_TEAM12_API AGP3_AICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGP3_AICharacterBase();

	UPROPERTY(BlueprintReadOnly)
		bool bIsPendingKill = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol_Points")
		TArray<FVector> PatrolPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target_Point")
		FVector TargetPoint;

	UPROPERTY(BlueprintReadOnly, Category = "Attack_Type")
	TEnumAsByte<enum AttackType> CurrentAttack = None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float Health = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float MaxHealth = 1000.f;

	bool HasAttacked = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Data")
		UBlackboardData* BlackboardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Data")
		UBehaviorTree* BehaviorTree;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
