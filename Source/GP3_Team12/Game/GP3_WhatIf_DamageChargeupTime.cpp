#include "GP3_WhatIf_DamageChargeupTime.h"
#include "Kismet/GameplayStatics.h"

void UGP3_WhatIfDamageChargeupTime::Setup_Implementation()
{

}

void UGP3_WhatIfDamageChargeupTime::OnWhatIfActivated_Implementation()
{
	//Reset timer
	Timer = ActiveTime;

	//Get Components
	Player = Cast<AGP3_CharacterBase>(UGameplayStatics::GetPlayerControllerFromID(this, 0)->GetPawn());
	Weapon = UGameplayStatics::GetPlayerControllerFromID(this, 0)->GetPawn()->FindComponentByClass<UGP3_PlayerWeapon>();

	//Cache default values
	Default_Damage = Weapon->ProjectileDamage; 
	Default_ChargeupTime = Weapon->ChargeUpTime; 

	//Set new values
	Weapon->ProjectileDamage = Damage; 
	Weapon->ChargeUpTime = ChargeupTime; 
	
	//Debug
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f,	FColor::Green, TEXT("DamageChargeupTime_Activated"));		
}

bool UGP3_WhatIfDamageChargeupTime::ShouldDeactivate_Implementation()
{
	return Timer <= 0.f;
}

UGP3_WhatIfBase* UGP3_WhatIfDamageChargeupTime::OnWhatIfDeactivated_Implementation()
{

	//Reset default values	
	//Player->bCanMove = true;
	Weapon->ProjectileDamage = Default_Damage; 
	Weapon->ChargeUpTime = Default_ChargeupTime; 
	
	//Cleanup
	Player = nullptr; 
	Weapon = nullptr; 
	
	return nullptr;
}