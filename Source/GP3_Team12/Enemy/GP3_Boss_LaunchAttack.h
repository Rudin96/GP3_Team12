#pragma once

#include "GameFramework/Actor.h"

#include "GP3_Boss_AttackBase.h"

#include "GP3_Boss_LaunchAttack.generated.h"

class UGP3_BossAttackFunctions; 

UCLASS(Blueprintable)
class UGP3_BossLaunchAttack : public UGP3_BossAttackBase {

	GENERATED_BODY()

public: 

	UGP3_BossLaunchAttack();

	UFUNCTION()
	void BeginPlay() override; 

	UFUNCTION(BlueprintCallable)
	void Attack_Implementation() override; 

private: 

	bool FailCheck(); 

public: 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	float MaxRadius = 3000; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	float JumpHeight = 2000; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Components")
	TSubclassOf<AActor> DecalClass = nullptr;

private: 

	int Iteration = 0; 
	int MaxIteration = 30; 

	UGP3_BossAttackFunctions* BossAttackFunctions = nullptr; 

};