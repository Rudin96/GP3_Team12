#include "GP3_WhatIf_Brain.h"
#include "Components/SceneComponent.h"
#include "GP3_Team12/Game/GP3_GameInstance.h"

AGP3_WhatIfBrain::AGP3_WhatIfBrain()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void AGP3_WhatIfBrain::BeginPlay()
{
	Super::BeginPlay();

	UGP3_GameInstance* GameInstance = UGP3_GameInstance::Get(GetWorld());

	if (!WhatIfs.IsEmpty())
	{
		for (auto w : WhatIfs)
		{
			ClosedWhatIfs.Add(NewObject<UGP3_WhatIfBase>(this, w));
		}
	}

	//if (!GameInstance->WhatifNames.IsEmpty())
	//{
	//	for (FString name : GameInstance->WhatifNames)
	//	{
	//		ChangeWhatIf(name);
	//	}
	//}
}

void AGP3_WhatIfBrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);		
}

void AGP3_WhatIfBrain::ChangeWhatIf(FString name)
{
	for (size_t i = 0; i < ClosedWhatIfs.Num(); i++)
	{
		if (ClosedWhatIfs[i]->WhatIfName == name)
		{
			//ActiveWhatIfs.Add(ClosedWhatIfs[i]);
			//ClosedWhatIfs.RemoveAt(i);
			//ActiveWhatIfs[ActiveWhatIfs.Num() - 1]->OnWhatIfActivated();
			ClosedWhatIfs[i]->OnWhatIfActivated();
			break;
		}
		else if (i == ClosedWhatIfs.Num() - 1)
		{
			GEngine->AddOnScreenDebugMessage(
				INDEX_NONE,
				3.f,
				FColor::Red,
				TEXT("Failed to find correct whatif: GP3_WhatIf_Brain.cpp"),
				true,
				FVector2D(1.f)
			);
			break;
		}
	}
	//Event
	OnChangeWhatIf.Broadcast(); 
}

TArray<UGP3_WhatIfBase*> AGP3_WhatIfBrain::GetWhatIfSenario()
{
	if (ClosedWhatIfs.Num() < 3)
		return ClosedWhatIfs;

	TArray<UGP3_WhatIfBase*> arr; 

	int a, b, c = 0; 
	a = FMath::RandRange(0, ClosedWhatIfs.Num() - 1);
	b = FMath::RandRange(0, ClosedWhatIfs.Num() - 1);
	c = FMath::RandRange(0, ClosedWhatIfs.Num() - 1);

	while (a == b || a == c || b == c)
	{
		if (a == b || a == c)
			a = FMath::RandRange(0, ClosedWhatIfs.Num() - 1);
		else
			c = FMath::RandRange(0, ClosedWhatIfs.Num() - 1);
	}

	arr.Add(ClosedWhatIfs[a]);
	arr.Add(ClosedWhatIfs[b]);
	arr.Add(ClosedWhatIfs[c]);

	OnNewWhatIfSenario.Broadcast(); 

	return arr; 
}