// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GP3_AICharacterBase.h"
#include "GP3_Boss_AICharacter.generated.h"

UCLASS()
class GP3_TEAM12_API ABossAICharacter : public AGP3_AICharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Kill();

public:	

	UFUNCTION(BlueprintPure, Meta = (DisplayName = "GetCurrentBoss", WorldContext = "WorldContext"), Category = "Boss|Utils")
    static ABossAICharacter* Get(UObject* WorldContext);

	UFUNCTION(BlueprintNativeEvent)
		void OnPendingKill();

	UPROPERTY(BlueprintReadOnly)
		bool bIsPendingKill = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float KillDelay = 1.f;


private:
	FTimerHandle KillHandle;
};
