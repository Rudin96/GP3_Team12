#pragma once

#include "GameFramework/Actor.h"

#include "GP3_Boss_AttackBase.h"

#include "GP3_Boss_ConespreadAttack.generated.h"

class USceneComponent; 
class UGP3_BossAttackFunctions;

UCLASS(Blueprintable)
class UGP3_BossConespreadAttack : public UGP3_BossAttackBase {

	GENERATED_BODY()

public: 

	UGP3_BossConespreadAttack(); 

	UFUNCTION()
	void BeginPlay() override; 

	UFUNCTION(BlueprintCallable)
	void Attack_Implementation() override;

private: 

	bool FailCheck(); 

public: 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	FVector SpawnOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	float StartAngle = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	float ProjectileVelocity = 1000; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	int Rings = 3; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Components")
	TSubclassOf<AActor> ProjectileClass = nullptr; 

private: 

	int ConeAmount = 4; 
	float RadiusFromBoss = 400; 
	float AngleIncreasement = 20; 

	USceneComponent* ConeStartLocation; 
	UGP3_BossAttackFunctions* BossAttackFunctions; 

};