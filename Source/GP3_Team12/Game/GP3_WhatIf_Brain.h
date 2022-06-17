#pragma once

#include "GameFramework/Actor.h"

#include "GP3_WhatIf_Base.h"

#include "GP3_WhatIf_Brain.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeWhatIfDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewWhatIfSenarioDelegate); 

class USceneComponent; 

UCLASS()
class AGP3_WhatIfBrain : public AActor {

	GENERATED_BODY()

public: 
	AGP3_WhatIfBrain();

	void BeginPlay() override; 
	void Tick(float DeltaTime) override; 
	
	UFUNCTION(BlueprintCallable)
	void ChangeWhatIf(FString name);
	
	UFUNCTION(BlueprintCallable)
	TArray<UGP3_WhatIfBase*> GetWhatIfSenario();

public: 

	UPROPERTY()
	USceneComponent* Root; 

	//All buffs
	UPROPERTY(EditDefaultsOnly, Category = "What If's")
	TArray<TSubclassOf<class UGP3_WhatIfBase>> WhatIfs;

	UPROPERTY(BlueprintAssignable)
	FOnChangeWhatIfDelegate OnChangeWhatIf;

	UPROPERTY(BlueprintAssignable)
	FOnNewWhatIfSenarioDelegate OnNewWhatIfSenario;

private: 

	UGP3_WhatIfBase* CurrentWhatIfEffect = nullptr;

	//All active buffs
	TArray<UGP3_WhatIfBase*> ActiveWhatIfs;

	//All available buffs
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UGP3_WhatIfBase*> ClosedWhatIfs; 
};