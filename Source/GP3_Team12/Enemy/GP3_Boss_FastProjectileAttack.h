#pragma once

#include "GameFramework/Actor.h"

#include "GP3_Boss_AttackBase.h"
#include "GP3_Boss_AttackFunctions.h"

#include "GP3_Boss_FastProjectileAttack.generated.h"

class USceneComponent; 

UCLASS()
class UGP3_FastProjectileAttack : public UGP3_BossAttackBase {

	GENERATED_BODY()

public: 

	void BeginPlay() override; 

	UFUNCTION(BlueprintCallable)
	void Attack_Implementation() override; 

private:

	bool FailCheck(); 

public: 

	UPROPERTY(EditAnywhere, Category = "Attack_Components")
	TSubclassOf<AActor> ProjectileClass; 

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	float ProjectileVelocity = 1000.f; 

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	float StartAngle = 0.f; 

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	float StartRadius = 100; 

private: 

	float AngleBetweenProjectiles = 10; 

	UGP3_BossAttackFunctions* BossAttackFunctions; 

	USceneComponent* ProjectileStartLocation;

};