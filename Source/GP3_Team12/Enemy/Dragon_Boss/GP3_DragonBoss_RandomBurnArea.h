#pragma once

#include "GameFramework/Actor.h"

#include "GP3_Team12/Enemy/GP3_Boss_AttackBase.h"

#include "GP3_DragonBoss_RandomBurnArea.generated.h"

UCLASS()
class UGP3_DragonBossRandomBurnArea : public UGP3_BossAttackBase {

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
	TSubclassOf<AActor> BurnAOEClass; 

private:

	TArray<AActor*> Platforms;

	float AOERadius = 1200.f; 

};