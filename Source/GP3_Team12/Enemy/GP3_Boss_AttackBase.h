#pragma once

#include "Components/ActorComponent.h"
#include "GP3_Boss_AttackBase.generated.h"

UENUM(Blueprintable)
enum AttackType 
{
	FirstAttack,
	SecondAttack,
	ThirdAttack,
	None
};

UCLASS(Blueprintable)
class UGP3_BossAttackBase : public UActorComponent {

	GENERATED_BODY()

public: 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack();
};