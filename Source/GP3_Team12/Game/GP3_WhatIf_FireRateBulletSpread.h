#pragma once

#include "GameFramework/Actor.h"

#include "GP3_WhatIf_Base.h"

#include "GP3_Team12/Weapon/GP3_PlayerWeapon.h"

#include "GP3_WhatIf_FireRateBulletSpread.generated.h"

UCLASS()
class UGP3_WhatIfFireRateBulletSpread : public UGP3_WhatIfBase {

	GENERATED_BODY()

public:

	void Setup_Implementation() override; 
	void OnWhatIfActivated_Implementation() override;
	bool ShouldDeactivate_Implementation() override;
	UGP3_WhatIfBase* OnWhatIfDeactivated_Implementation() override;


public: 

	UPROPERTY(EditDefaultsOnly, Category = Properties)
	float FireRate = 0; 

	UPROPERTY(EditDefaultsOnly, Category = Properties)
	float BulletSpread = 0; 

private: 

	UGP3_PlayerWeapon* Weapon = nullptr;

	float timer = 0;

	float Default_FireRate = 0; 

};