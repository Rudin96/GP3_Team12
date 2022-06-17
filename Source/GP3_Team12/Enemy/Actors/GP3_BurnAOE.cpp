#include "GP3_BurnAOE.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

AGP3_BurnAOE::AGP3_BurnAOE()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	RootComponent = Root; 

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Root); 
}