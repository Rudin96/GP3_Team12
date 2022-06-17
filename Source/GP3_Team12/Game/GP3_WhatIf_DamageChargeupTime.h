#pragma once

#include "GP3_WhatIf_Base.h"

#include "GP3_Team12/Character/GP3_CharacterBase.h"
#include "GP3_Team12/Weapon/GP3_PlayerWeapon.h"

#include "GP3_WhatIf_DamageChargeupTime.generated.h"

UCLASS()
class UGP3_WhatIfDamageChargeupTime : public UGP3_WhatIfBase  {

	GENERATED_BODY()

public:

	void Setup_Implementation() override;
	void OnWhatIfActivated_Implementation() override;
	bool ShouldDeactivate_Implementation() override;
	UGP3_WhatIfBase* OnWhatIfDeactivated_Implementation() override;

public: 

	UPROPERTY(EditDefaultsOnly, Category = Properties)
	float Damage = 0; 

	UPROPERTY(EditDefaultsOnly, Category = Properties)
	float ChargeupTime = 5; 


private: 

	AGP3_CharacterBase* Player = nullptr;
	UGP3_PlayerWeapon* Weapon = nullptr; 

	float Timer = 0;

	//Default values to reset to
	float Default_Damage = 0; 
	float Default_ChargeupTime = 0; 
};