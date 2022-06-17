#pragma once

#include "GameFramework/Actor.h"

#include "GP3_Boss_AttackBase.h"

#include "GP3_Boss_ProjectileRingAttack.generated.h"

class UGP3_BossAttackFunctions; 

UCLASS()
class UGP3_ProjectileRingAttack : public UGP3_BossAttackBase {

	GENERATED_BODY()

public:

	UFUNCTION()
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
	int ProjectileAmount = 36; 

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	float StartRadius = 500; 

private:

	UGP3_BossAttackFunctions* BossAttackFunctions; 

	USceneComponent* ProjectileStartLocation; 
};