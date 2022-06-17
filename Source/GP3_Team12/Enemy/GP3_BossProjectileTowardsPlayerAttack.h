#pragma once

#include "GP3_Boss_AttackBase.h"

#include "GP3_BossProjectileTowardsPlayerAttack.generated.h"

class AGP3_CharacterBase; 

UCLASS()
class UGP3_BossProjectileTowardsPlayerAttack : public UGP3_BossAttackBase {

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
	float StartRadius = 300; 

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	float ProjectileVelocity = 1000; 

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	int ProjectileAmount = 3; 
private:

	float AngleOfAttack = 90.f;

	AGP3_CharacterBase* Player; 

};