#include "GP3_DragonBoss_RandomBurnArea.h"
#include "GP3_DragonBoss_PlatformHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

void UGP3_DragonBossRandomBurnArea::BeginPlay()
{
	Super::BeginPlay(); 

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP3_DragonBossPlatformHandler::StaticClass(), Platforms);		
}

void UGP3_DragonBossRandomBurnArea::Attack_Implementation()
{
	if (!FailCheck())
		return; 

	int rand = FMath::RandRange(0, Platforms.Num() - 1); 

	AGP3_DragonBossPlatformHandler* platform = Cast<AGP3_DragonBossPlatformHandler>(Platforms[rand]); 

	float platformRadius = platform->FindComponentByClass<USphereComponent>()->GetUnscaledSphereRadius();

	float radius = FMath::RandRange(platformRadius / 2, platformRadius);
	radius -= AOERadius / 1.3f;

	float angle = FMath::RandRange(0, 360); 

	FVector spawnLocation = FVector(
		platform->GetActorLocation().X + radius * cos(FMath::DegreesToRadians(angle)),
		platform->GetActorLocation().Y + radius * sin(FMath::DegreesToRadians(angle)),
		platform->GetActorLocation().Z
	);

	GetWorld()->SpawnActor<AActor>(BurnAOEClass, spawnLocation, FRotator::ZeroRotator); 
}

bool UGP3_DragonBossRandomBurnArea::FailCheck()
{
	if (Platforms.IsEmpty())
		return false; 
	if (BurnAOEClass == nullptr)
		return false; 
	
	return true; 
}