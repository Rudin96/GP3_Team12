#pragma once

#include "Components/ActorComponent.h"

#include "GP3_Team12/Enemy/GP3_Boss_AttackBase.h"
#include "GP3_DragonBoss_PlatformHandler.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"
#include "GP3_Team12/Game/GP3_GameInstance.h"
#include "GP3_DragonBoss_IslandBurnAttack.generated.h"

UCLASS()
class UGP3_DragonBossIslandBurnAttack : public UGP3_BossAttackBase {

	GENERATED_BODY()

public: 

	UGP3_DragonBossIslandBurnAttack(); 

	UFUNCTION()
	void BeginPlay() override; 
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void Attack_Implementation() override; 

private: 

	UFUNCTION()
	void StartDamageTick(); 
	UFUNCTION()
	void PauseDamageTick();
	
	void CheckForPlayer(); 
	void OnAttackFinished(); 
	bool FailCheck(); 

public: 

	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	float Lifetime = 14.f; 
	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	float DamageDelay = 1.f;
	UPROPERTY(EditAnywhere, Category = "Attack_Settings")
	float Damage = 1; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack_Settings")
		TSubclassOf<AActor> DamageActor;

	UPROPERTY(BlueprintReadWrite)
	AGP3_DragonBossPlatformHandler* ActivePlatform; 

private:

	AGP3_CharacterBase* Player; 

	UFUNCTION()
	void SetActivePlatform(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<AGP3_DragonBossPlatformHandler*> Platforms;

	bool PlayerOnIsland = false; 
	float Timer = 0.f; 
	float LifetimeTimer = 0.f; 

	bool IsAlive = false; 
};