// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../../Character/GP3_CharacterBase.h"
#include "../../AI/BossAI/GP3_Boss_AICharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GP3_ProjectileBase.generated.h"

UCLASS()
class GP3_TEAM12_API AGP3_ProjectileBase : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle LifetimeHandle;

	FVector SpawnPosition;

	void ApplyCurveOffset();

public:
	// Sets default values for this actor's properties
	AGP3_ProjectileBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* Collision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCurveFloat* ProjectilePathCurveVertical;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCurveFloat* ProjectilePathCurveHorizontal;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileHit();

	UFUNCTION()
	void SetSpeed(float NewSpeed) const;

	UFUNCTION()
	void SetDamage(float NewDamage);


	void DestroySelf();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ProjectileSpeed = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Lifetime = 5.f;
};
