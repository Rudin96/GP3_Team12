#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GP3_Team12/Character/GP3_CharacterBase.h"

#include "GP3_WhatIf_Base.generated.h"

UCLASS(Blueprintable)
class UGP3_WhatIfBase : public UObject {

	GENERATED_BODY()

public: 
	UFUNCTION(BlueprintNativeEvent)
	void Setup();

	UFUNCTION(BlueprintNativeEvent)
    void OnWhatIfActivated(); 

	UFUNCTION(BlueprintNativeEvent)
	bool ShouldDeactivate(); 

	UFUNCTION(BlueprintNativeEvent)
	UGP3_WhatIfBase* OnWhatIfDeactivated();

	UFUNCTION(Blueprintable)
	AActor* GetOwner() { return Cast<AActor>(GetOuter()); }

	UFUNCTION(BlueprintCallable)
	AGP3_CharacterBase* GetPlayer(); 

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Properties)
	FString WhatIfName; 

	UPROPERTY(EditDefaultsOnly, Category = Properties)
	float ActiveTime = 20; 	
};