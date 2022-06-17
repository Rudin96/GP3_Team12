// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Projectile/GP3_ProjectileBase.h"
#include "GP3_PlayerWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartChargingDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopChargingDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootAnimationDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailedToShootInstantlyDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_TEAM12_API UGP3_PlayerWeapon : public UActorComponent
{
	GENERATED_BODY()

private:
	void PrepareNextShot();
	void StartCharging();
	void StopCharging();
	void StartShootDelay();
	void StopShootDelay();
	void ShootAnimationDelayStart();
	void ShootAnimationDelayStop();
	void EnableShooting();
	void DisableShooting();
	void ResetNextShotBonusDamage();
	void ModifyCurveAmplitude();
	void InitializeCurves();

	FTimerHandle ShootDelayTimerHandle;
	FTimerHandle ShootAgainTimerHandle;
	FTimerHandle StartChargingTimerHandle;
	FTimerHandle StopChargingTimerHandle;
	FTimerHandle AnimationDelayTimerHandle;

	bool bIsShooting = false;
	bool bIsCharging = false;
	bool bCanShoot = true;
	
	float NextShotDamageMultiplier = 1.f;
	float NextShotAddedDamage = 0.f;

	FVector ProjectileBaseScale;

	UPROPERTY()
	UCurveFloat* UnmodifiedProjectilePathCurveVertical;

	UPROPERTY()
	UCurveFloat* UnmodifiedProjectilePathCurveHorizontal;

public:
	// Sets default values for this component's properties
	UGP3_PlayerWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSize = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileMaxSize = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = 1))
	int ProjectilesPerShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = 0.f, ClampMax = 90.f))
	float ProjectileSpread = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileRange = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DelayBetweenShots = 1.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ChargeUpTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector SpawnOffset = FVector(150.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AGP3_ProjectileBase> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UCurveFloat* ProjectilePathCurveVertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UCurveFloat* ProjectilePathCurveHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector2D ProjectilePathCurveAmplitude = FVector2D(1000.f, 1000.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float ShootAnimationDelay = 0.3;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	// UAnimSequence* ShootAnimation;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool IsCharging() const
	{
		return bIsCharging;
	}

	UFUNCTION(BlueprintCallable)
	bool IsShooting() const
	{
		return bIsShooting;
	}

	UFUNCTION(BlueprintCallable)
	bool CanShoot() const
	{
		return bCanShoot;
	}

	UFUNCTION(BlueprintCallable)
	bool CanShootInstantly();

	UFUNCTION(BlueprintCallable)
	void ShootOnce();

	UFUNCTION(BlueprintCallable)
	void StartShooting();

	UFUNCTION(BlueprintCallable)
	void StopShooting();

	UFUNCTION(BlueprintCallable)
	void SetDelayBetweenShots(float NewDelay);

	UFUNCTION(BlueprintCallable)
	void SetNextShotDamageMultiplier(float DamageMultiplier);

	UFUNCTION(BlueprintCallable)
	void SetNextShotAddedDamage(float AddedDamage);

	UFUNCTION(BlueprintCallable)
	void SetProjectilePathCurveVertical(UCurveFloat* Curve);

	UFUNCTION(BlueprintCallable)
	void SetProjectilePathCurveHorizontal(UCurveFloat* Curve);

	UFUNCTION(BlueprintCallable)
	void AddProjectilePathCurveVertical(UCurveFloat* Curve);

	UFUNCTION(BlueprintCallable)
	void AddProjectilePathCurveHorizontal(UCurveFloat* Curve);
	
	UFUNCTION(BlueprintCallable)
	void SetProjectilePathCurveAmplitude(FVector2D NewAmplitude);

	
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnStartChargingDelegate OnStartCharging;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnStopChargingDelegate OnStopCharging;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnShootDelegate OnShoot;
	
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnShootAnimationDelegate OnShootAnimation;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnFailedToShootInstantlyDelegate OnFailedToShootInstantly;
};
