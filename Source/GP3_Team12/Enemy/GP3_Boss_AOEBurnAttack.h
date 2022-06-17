#pragma once

#include "GameFramework/Actor.h"

#include "GP3_Boss_AttackBase.h"

#include "GP3_Boss_AOEBurnAttack.generated.h"

class UGP3_BossAttackFunctions;

UCLASS(Blueprintable)
class UGP3_BossAOEBurnAttack : public UGP3_BossAttackBase {

	GENERATED_BODY()

public: 

	UGP3_BossAOEBurnAttack();

	UFUNCTION()
	void BeginPlay() override; 

	UFUNCTION(BlueprintCallable)
	void Attack_Implementation() override; 

private:
	bool FailCheck(); 

public: 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	float MaxRadius = 3000.f; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Components")
	TSubclassOf<AActor> AOEActorClass = nullptr; 

private:

	int MaxIteration = 30; 
	int Iteration = 0; 

	UGP3_BossAttackFunctions* BossAttackFunctions = nullptr; 
};