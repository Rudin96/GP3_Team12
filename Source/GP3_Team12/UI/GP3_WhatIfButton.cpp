#include "GP3_WhatIfButton.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "../Game/GP3_GameInstance.h"

UGP3_WhatIfButton::UGP3_WhatIfButton()
{
	OnClicked.AddDynamic(this, &UGP3_WhatIfButton::OnClick);
	//Bind function
	load.AddDynamic(this, &UGP3_WhatIfButton::LoadGameFile);
}

void UGP3_WhatIfButton::OnClick()
{
	load.Broadcast(WhatIfName);
	UGP3_GameInstance* GameInstance = UGP3_GameInstance::Get(GetWorld());
	GameInstance->WhatifNames.Add(WhatIfName);
}

void UGP3_WhatIfButton::LoadGameFile(FString Name)
{
	if (Brain == nullptr)
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP3_WhatIfBrain::StaticClass(), foundActors);
		if (foundActors.IsEmpty())
		{
			Brain = nullptr;
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Cyan, TEXT("WHAT IF BRAIN NOT FOUND, PLEASE FIX!"));
			return;
		}
		else
		{
			Brain = Cast<AGP3_WhatIfBrain>(foundActors[0]);
		}
	}

	Brain->ChangeWhatIf(Name);	
}

