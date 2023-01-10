// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_PlayerWeapon.h"
#include "Projectile/GP3_ProjectileBase.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGP3_PlayerWeapon::UGP3_PlayerWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGP3_PlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	ProjectileBaseScale = ProjectileClass.GetDefaultObject()->ProjectileMesh->GetRelativeScale3D();
	InitializeCurves();
}

// Called every frame
void UGP3_PlayerWeapon::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsCharging())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red,TEXT("Charging"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red,TEXT("Not Charging"));
	}
}

void UGP3_PlayerWeapon::ShootOnce()
{
	// if (!CanShoot() || !IsShooting())
	// {
	// 	return;
	// }
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,TEXT("Shoot!"));
	for (int i = 0; i < ProjectilesPerShot; ++i)
	{
		FRotator Spread = FRotator(0.f, FMath::RandRange(-ProjectileSpread, ProjectileSpread), 0.f);
		FRotator Rotation = GetOwner()->GetActorRotation() + Spread;
		const float Scale = FMath::Clamp(ProjectileSize, 0.f, ProjectileMaxSize);
		FTransform Transform = FTransform(Rotation, GetOwner()->GetActorLocation() + Rotation.RotateVector(SpawnOffset),
		                                  ProjectileBaseScale * Scale);
		if (AGP3_ProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AGP3_ProjectileBase>(
			ProjectileClass, Transform))
		{
			Projectile->SetSpeed(ProjectileSpeed);
			Projectile->SetDamage(ProjectileDamage * NextShotDamageMultiplier + NextShotAddedDamage);
			Projectile->Lifetime = ProjectileRange / ProjectileSpeed;
			Projectile->ProjectileMesh->SetWorldScale3D(ProjectileBaseScale * Scale);
			Projectile->ProjectilePathCurveVertical = ProjectilePathCurveVertical;
			Projectile->ProjectilePathCurveHorizontal = ProjectilePathCurveHorizontal;

			UGameplayStatics::FinishSpawningActor(Projectile, Transform);
		}
	}
	ResetNextShotBonusDamage();
	StartShootDelay();
	OnShoot.Broadcast();
}

void UGP3_PlayerWeapon::StartShooting()
{
	bIsShooting = true;
	PrepareNextShot();
}

void UGP3_PlayerWeapon::StopShooting()
{
	bIsShooting = false;
	StopCharging();
	GetWorld()->GetTimerManager().ClearTimer(StopChargingTimerHandle);
}

void UGP3_PlayerWeapon::SetDelayBetweenShots(const float NewDelay)
{
	DelayBetweenShots = NewDelay;
}


void UGP3_PlayerWeapon::SetNextShotDamageMultiplier(const float DamageMultiplier)
{
	NextShotDamageMultiplier = DamageMultiplier;
}

void UGP3_PlayerWeapon::SetNextShotAddedDamage(const float AddedDamage)
{
	NextShotAddedDamage = AddedDamage;
}

void UGP3_PlayerWeapon::SetProjectilePathCurveVertical(UCurveFloat* Curve)
{
	UnmodifiedProjectilePathCurveVertical = Curve;
	ModifyCurveAmplitude();
}

void UGP3_PlayerWeapon::SetProjectilePathCurveHorizontal(UCurveFloat* Curve)
{
	UnmodifiedProjectilePathCurveHorizontal = Curve;
	ModifyCurveAmplitude();
}

void UGP3_PlayerWeapon::AddProjectilePathCurveVertical(UCurveFloat* Curve)
{
	UCurveFloat* NewCurve = DuplicateObject(Curve, GetOuter(), "ModifiedCurveV");
	TArray<FRichCurveKey> Keys = UnmodifiedProjectilePathCurveVertical->FloatCurve.GetCopyOfKeys();
	for (const FRichCurveKey Key : Keys)
	{
		NewCurve->FloatCurve.UpdateOrAddKey(Key.Time, Key.Value + NewCurve->GetFloatValue(Key.Time));
	}
	UnmodifiedProjectilePathCurveVertical = NewCurve;
	ModifyCurveAmplitude();
}

void UGP3_PlayerWeapon::AddProjectilePathCurveHorizontal(UCurveFloat* Curve)
{
	UCurveFloat* NewCurve = DuplicateObject(Curve, GetOuter(), "ModifiedCurveH");
	TArray<FRichCurveKey> Keys = UnmodifiedProjectilePathCurveHorizontal->FloatCurve.GetCopyOfKeys();
	for (const FRichCurveKey Key : Keys)
	{
		NewCurve->FloatCurve.UpdateOrAddKey(Key.Time, Key.Value + NewCurve->GetFloatValue(Key.Time));
	}
	UnmodifiedProjectilePathCurveHorizontal = NewCurve;
	ModifyCurveAmplitude();
}

void UGP3_PlayerWeapon::SetProjectilePathCurveAmplitude(FVector2D NewAmplitude)
{
	ProjectilePathCurveAmplitude = NewAmplitude;
	ModifyCurveAmplitude();
}

void UGP3_PlayerWeapon::PrepareNextShot()
{
	if (ChargeUpTime > 0.f)
	{
		StartCharging();
	}
	else if (CanShootInstantly())
	{
		// ShootOnce();
		ShootAnimationDelayStart();
	}
	else
	{
		OnFailedToShootInstantly.Broadcast();
	}
}

void UGP3_PlayerWeapon::StartCharging()
{
	bIsCharging = true;
	OnStartCharging.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(StopChargingTimerHandle, this, &UGP3_PlayerWeapon::StopCharging,
	                                       ChargeUpTime, false);
}

void UGP3_PlayerWeapon::StopCharging()
{
	bIsCharging = false;
	OnStopCharging.Broadcast();
	if (IsShooting())
	{
		// ShootOnce();
		ShootAnimationDelayStart();
	}
}

void UGP3_PlayerWeapon::StartShootDelay()
{
	GetWorld()->GetTimerManager().SetTimer(ShootDelayTimerHandle, this, &UGP3_PlayerWeapon::StopShootDelay,
	                                       DelayBetweenShots, false);
}

void UGP3_PlayerWeapon::StopShootDelay()
{
	if (IsShooting())
	{
		PrepareNextShot();
	}
}

void UGP3_PlayerWeapon::ShootAnimationDelayStart()
{
	if (!CanShoot() || !IsShooting())
	{
		return;
	}
	if (GetWorld()->GetTimerManager().GetTimerRemaining(AnimationDelayTimerHandle) > 0.f)
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(AnimationDelayTimerHandle, this,
	                                       &UGP3_PlayerWeapon::ShootAnimationDelayStop, ShootAnimationDelay,
	                                       false);
	OnShootAnimation.Broadcast();
}

void UGP3_PlayerWeapon::ShootAnimationDelayStop()
{
	ShootOnce();
}


void UGP3_PlayerWeapon::EnableShooting()
{
	bCanShoot = true;
}

void UGP3_PlayerWeapon::DisableShooting()
{
	bCanShoot = false;
}

bool UGP3_PlayerWeapon::CanShootInstantly()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(ShootDelayTimerHandle) > 0.f ? false : true;
}

void UGP3_PlayerWeapon::ResetNextShotBonusDamage()
{
	NextShotDamageMultiplier = 1.f;
	NextShotAddedDamage = 0.f;
}

void UGP3_PlayerWeapon::ModifyCurveAmplitude()
{
	TArray<FRichCurveKey> Keys = UnmodifiedProjectilePathCurveVertical->FloatCurve.GetCopyOfKeys();
	for (FRichCurveKey Key : Keys)
	{
		Keys.FindByKey(Key)->Value *= ProjectilePathCurveAmplitude.Y;
	}
	ProjectilePathCurveVertical = DuplicateObject(UnmodifiedProjectilePathCurveVertical, GetOuter(),
	                                              "ModifiedCurveVertical");
	ProjectilePathCurveVertical->FloatCurve.SetKeys(Keys);

	Keys = UnmodifiedProjectilePathCurveHorizontal->FloatCurve.GetCopyOfKeys();
	for (FRichCurveKey Key : Keys)
	{
		Keys.FindByKey(Key)->Value *= ProjectilePathCurveAmplitude.X;
	}
	ProjectilePathCurveHorizontal = DuplicateObject(UnmodifiedProjectilePathCurveHorizontal, GetOuter(),
	                                                "ModifiedCurveHorizontal");
	ProjectilePathCurveHorizontal->FloatCurve.SetKeys(Keys);
}

void UGP3_PlayerWeapon::InitializeCurves()
{
	if (!ProjectilePathCurveVertical)
	{
		ProjectilePathCurveVertical = NewObject<UCurveFloat>();
	}
	if (!ProjectilePathCurveHorizontal)
	{
		ProjectilePathCurveHorizontal = NewObject<UCurveFloat>();
	}
	UnmodifiedProjectilePathCurveVertical = ProjectilePathCurveVertical;
	UnmodifiedProjectilePathCurveHorizontal = ProjectilePathCurveHorizontal;
	ModifyCurveAmplitude();
}
