#include "GP3_WhatIf_FireRateBulletSpread.h"
#include "Kismet/GameplayStatics.h"

void UGP3_WhatIfFireRateBulletSpread::Setup_Implementation()
{
}

void UGP3_WhatIfFireRateBulletSpread::OnWhatIfActivated_Implementation()
{
	//Reset timer
	timer = ActiveTime; 
	
	//Get Components
	Weapon = UGameplayStatics::GetPlayerControllerFromID(this, 0)->GetPawn()->FindComponentByClass<UGP3_PlayerWeapon>(); 
	
	//Cache default values
	Default_FireRate = Weapon->DelayBetweenShots;

	//Set new values
	Weapon->DelayBetweenShots = FireRate; 

	//Debug
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, TEXT("FireRateBulletSpread_Activated"));
}

bool UGP3_WhatIfFireRateBulletSpread::ShouldDeactivate_Implementation()
{
	return timer <= 0.f; 
}

UGP3_WhatIfBase* UGP3_WhatIfFireRateBulletSpread::OnWhatIfDeactivated_Implementation()
{
	//Reset to default values
	Weapon->DelayBetweenShots = Default_FireRate; 

	//Cleanup
	Weapon = nullptr; 

	//Debug
	GEngine->AddOnScreenDebugMessage(
		INDEX_NONE,
		3.f,
		FColor::Green,
		TEXT("FireRateBulletSpread_Deactivated"),
		true,
		FVector2D(1.f)
	);

	return nullptr; 
}