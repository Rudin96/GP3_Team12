// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "GP3_CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UGP3_CharacterMovementBase;
class UGP3_PlayerWeapon;

UCLASS()
class GP3_TEAM12_API AGP3_CharacterBase : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* Collision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bCanPerformActions = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bCanShoot = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGP3_CharacterMovementBase* MovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGP3_PlayerWeapon* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite = Category = "Camera")
		float MinBoomLength = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite = Category = "Camera")
		float MaxBoomLength = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite = Category = "Camera")
		float ZoomAmount = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite = Category = "Camera")
		float ZoomLerpSpeed = 20.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsInvulnerable = false;

private:

	float NewTargetArmLength = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this pawn's properties
	AGP3_CharacterBase();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnDeath();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
		FVector GetPlayerVelocity();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
		float GetMovementSpeed();

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetMovementSpeed(float NewSpeed);

	UFUNCTION(BlueprintCallable, Category = "Death")
		void KillPlayer();

	UFUNCTION()
	void AddForce(const FVector& Dir);

	UFUNCTION()
	void AddImpulse(const FVector& Dir);

protected:

	void HandleMoveRight(float Value);
	void HandleMoveForward(float Value);

	void HandleLookUp(float Value);
	void HandleLookRight(float Value);

	void HandleJump();
	void StopJump();
	void HandlePrimaryAction();
	void HandleRollAction();
	void StopPrimaryAction();

	void HandleSecondaryAction();

	void HandleZoom(float Value);
};
