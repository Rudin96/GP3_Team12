#pragma once	

#include "GameFramework/Actor.h"

#include "GP3_Boss_AttackBase.h"

#include "GP3_Boss_ProjectileLaunchAttack.generated.h"

class USceneComponent; 
class UGP3_BossAttackFunctions;

UCLASS(Blueprintable)
class UGP3_BossProjectileLaunchAttack : public UGP3_BossAttackBase {

	GENERATED_BODY()

public: 

	UGP3_BossProjectileLaunchAttack();
	
	UFUNCTION(BlueprintCallable)
	void Attack_Implementation() override;

	UFUNCTION()
	void BeginPlay() override; 	

private:

	bool FailCheck(); 

public: 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	int ProjectileAmount = 10; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	float Radius = 1000; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	float ProjectileLaunchHeight = 1000; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	float AngleOfAttack = 360; 

	UPROPERTY(EditDefaultsOnly, Category = "Attack_Settings")
	TSubclassOf<AActor> ProjectileClass;	

private: 

	UGP3_BossAttackFunctions* BossAttackFunctions = nullptr; 

	USceneComponent* ProjectileStartLocation; 
};
