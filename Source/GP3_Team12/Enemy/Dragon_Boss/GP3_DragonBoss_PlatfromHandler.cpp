#include "GP3_DragonBoss_PlatformHandler.h"
#include "Components/SphereComponent.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"

AGP3_DragonBossPlatformHandler::AGP3_DragonBossPlatformHandler()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere")); 
	RootComponent = Sphere; 

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGP3_DragonBossPlatformHandler::FireBeginOverlapEvents);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AGP3_DragonBossPlatformHandler::FireEndOverlapEvents);
}

void AGP3_DragonBossPlatformHandler::FireBeginOverlapEvents(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AGP3_CharacterBase>(OtherActor))
		OnPlayerEnterIsland.Broadcast();
}

void AGP3_DragonBossPlatformHandler::FireEndOverlapEvents(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AGP3_CharacterBase>(OtherActor))
		OnPlayerExitIsland.Broadcast();
}