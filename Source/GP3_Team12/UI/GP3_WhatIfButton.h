#pragma once

#include "Components/Button.h"

#include "GP3_Team12/Game/GP3_WhatIf_Brain.h"

#include "GP3_WhatIfButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadDelegate, FString, name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClickDelegate);

UCLASS()
class UGP3_WhatIfButton : public UButton {

	GENERATED_BODY()

public:
	UGP3_WhatIfButton();

	UPROPERTY()
	FLoadDelegate load;

	UPROPERTY()
	FClickDelegate click;

	UFUNCTION()
	void OnClick();

	UFUNCTION()
	void LoadGameFile(FString Name);

public:

	/// <summary>
	/// Must be set to the same name as the respective WhatIf BP
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Button_Settings")
	FString WhatIfName; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Button_Settings")
	FText UIButtonLabel; 

private: 

	AGP3_WhatIfBrain* Brain = nullptr; 
};