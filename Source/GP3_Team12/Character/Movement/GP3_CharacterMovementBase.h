// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../GP3_CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GP3_CharacterMovementBase.generated.h"

UENUM(Blueprintable)
enum ECharacterMovementMode
{
	MOVEMENT_NONE UMETA(DisplayName = "None"),
	MOVEMENT_WALKING UMETA(DisplayName = "Walking"),
	MOVEMENT_FALLING UMETA(DisplayName = "Falling"),
	MOVEMENT_DASHING UMETA(DisplayName = "Dashing"),
	MOVEMENT_IDLE UMETA(DisplayName = "Idle")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopDashDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLandedDelegate);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM12_API UGP3_CharacterMovementBase : public UActorComponent
{
	GENERATED_BODY()

private:

	AGP3_CharacterBase* GetCharacter() { return Cast<AGP3_CharacterBase>(GetOwner()); }

	void UpdateMovement(float DeltaSeconds);
	void ApplyGravity(float DeltaSeconds);
	void ApplyDeceleration(float DeltaSeconds);

	void PerformDash();

	void CheckConsoleVariables();

	void RechargeDashCooldowns();

	FVector DashDir;

	FTimerHandle CooldownTimerHandle;

	FVector DashStart;
	FVector DashEnd;
	
	float CurrentDashTime = 0.f;
	bool bWantsToDash = false;
	bool bIsDashing = false;
	bool bWantsToJump = false;


public:	

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<enum ECharacterMovementMode> MovementMode = MOVEMENT_WALKING;

	UPROPERTY(BlueprintReadWrite)
	bool bHasJumped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking", meta = (ConsoleVariable = "character.movementspeed"))
	float MaxMovementSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking")
	float StepupHeight = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking")
	float MaxWalkableAngle = 45.f;

	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Walking", meta = (ConsoleVariable = "character.acceleration"))
	float Acceleration = 400.f;

	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Walking", meta = (ConsoleVariable = "character.deceleration"))
	float Deceleration = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking", meta=(ClampMin="0.0", ClampMax="1.0"))
	float WalkableFloorZ = .9f;

	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Jumping", meta = (ConsoleVariable = "character.jumpstrength"))
	float JumpStrength = 420.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
	int MaxJumpCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depenetration", meta=(ClampMin="0.0", ClampMax="0.5"))
	float DepenThreshold = .05;

	UPROPERTY(BlueprintReadOnly)
	int CurrentJumpCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Falling")
	float GravityScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Falling", meta=(ClampMin="0.0", ClampMax="1.0"))
	float AirControl = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Falling | GroundCheck")
	float GroundedCheckThreshold = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Falling | GroundCheck")
	float GroundedCheckDistance = 10.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash")
	float DashRechargeRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash")
	int MaxDashCount = 5;

	UPROPERTY(BlueprintReadWrite, Category="Dash")
	int DashCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash")
	float DashLength = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	UCurveFloat* DashCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Roll")
	UCurveFloat* RollCurve;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dash | Cooldowns")
	float ElapsedDashRechargeTime() { return GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle); }

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Input")
	FVector InputVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	FVector Velocity;
protected:

	virtual void BeginPlay() override;

public:	

	UGP3_CharacterMovementBase();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void AddForce(const FVector& Dir);

	UFUNCTION(BlueprintCallable)
	void AddImpulse(FVector Dir);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMovementSpeed(){ return Velocity.Length(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetVelocity() { return Velocity; }

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void StopJump();

	UPROPERTY(BlueprintAssignable, Category = "Falling")
	FOnLandedDelegate OnLandedDelegate;

	void OnLanded();

	UPROPERTY(BlueprintAssignable, Category = "Dash")
	FOnDashDelegate OnBeginDash;

	UPROPERTY(BlueprintAssignable, Category = "Dash")
	FOnDashDelegate OnStopDash;

	UFUNCTION(BlueprintCallable)
	void BeginDash();
};
