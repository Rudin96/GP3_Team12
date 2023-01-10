#include "GP3_DragonBoss_IslandBurnAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GP3_Team12/AI/BossAI/GP3_Boss_AICharacter.h"
#include "Components/SphereComponent.h"


UGP3_DragonBossIslandBurnAttack::UGP3_DragonBossIslandBurnAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGP3_DragonBossIslandBurnAttack::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> foundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		AGP3_DragonBossPlatformHandler::StaticClass(), foundActors);

	if (!foundActors.IsEmpty())
	{
		for (auto actor : foundActors)
		{
			Platforms.Add(Cast<AGP3_DragonBossPlatformHandler>(actor));
		}
	}
	else
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, "No Dragonboss-PlatformHandlers in level");

	
		Player = UGP3_GameInstance::GetPlayer(GetWorld());
		if (Player == nullptr)
			return;

		Player->Collision->OnComponentBeginOverlap.AddDynamic(this, &UGP3_DragonBossIslandBurnAttack::SetActivePlatform);
}

void UGP3_DragonBossIslandBurnAttack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!FailCheck() && !IsAlive)
		return;

	if (PlayerOnIsland && IsAlive)
	{
		if (Timer > 0)
			Timer -= DeltaTime;
		else
		{		
			//ABossAICharacter* boss = ABossAICharacter::Get(GetWorld());	
			
			//Change player->GetController() to boss->GetController() 
			Player->TakeDamage(Damage, FDamageEvent(), Player->GetController(), GetOwner());
			UE_LOG(LogTemp, Warning, TEXT("Damaging player c++"));
			
			Timer = DamageDelay;
		}
	}

	LifetimeTimer -= DeltaTime;	
}

void UGP3_DragonBossIslandBurnAttack::SetActivePlatform(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGP3_DragonBossPlatformHandler* Platform = Cast<AGP3_DragonBossPlatformHandler>(OtherActor))
		ActivePlatform = Platform;
}

void UGP3_DragonBossIslandBurnAttack::Attack_Implementation()
{
	if (!FailCheck() && !IsAlive)
		return;

	//Set timer to initial time
	Timer = DamageDelay;
	//Set lifetime timer to initial time
	LifetimeTimer = Lifetime;

	//Get one of 3 platforms at random to cover it in "fire"
	//int rand = FMath::RandRange(0, Platforms.Num() - 1);
	//ActivePlatform = Platforms[rand];

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if(DamageActor != nullptr && ActivePlatform != nullptr)
		AActor* DamageDealerActor = GetWorld()->SpawnActor<AActor>
		(DamageActor, FTransform(FRotator(0.f), ActivePlatform->GetActorLocation() + FVector::UpVector * 45.f, FVector(4.f)), SpawnParams);

 	if (ActivePlatform != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, ActivePlatform->GetFName().ToString());
		ActivePlatform->OnPlayerEnterIsland.AddDynamic(this, &UGP3_DragonBossIslandBurnAttack::StartDamageTick);
		ActivePlatform->OnPlayerExitIsland.AddDynamic(this, &UGP3_DragonBossIslandBurnAttack::PauseDamageTick);
		CheckForPlayer(); 
	}

	IsAlive = true;
}

void UGP3_DragonBossIslandBurnAttack::CheckForPlayer()
{
	USphereComponent* sphereComp = ActivePlatform->FindComponentByClass<USphereComponent>();
	
	float distance = FVector::Distance(Player->GetActorLocation(), ActivePlatform->GetActorLocation());

	if (distance <= sphereComp->GetScaledSphereRadius())
		PlayerOnIsland = true; 
}

//OnEvent function 
void UGP3_DragonBossIslandBurnAttack::StartDamageTick()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, TEXT("Entered Island")); 
	PlayerOnIsland = true;
}
//OnEvent function
void UGP3_DragonBossIslandBurnAttack::PauseDamageTick()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, TEXT("Exited Island"));
	PlayerOnIsland = false;
}

//Called when the attack has run for the duration of it's lifetime
void UGP3_DragonBossIslandBurnAttack::OnAttackFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("OnAttackFinished")); 
	PlayerOnIsland = false; 
	
	if (ActivePlatform != nullptr)
	{
		for (auto platform : Platforms)
		{
			platform->OnPlayerEnterIsland.RemoveDynamic(this, &UGP3_DragonBossIslandBurnAttack::StartDamageTick);
			platform->OnPlayerExitIsland.RemoveDynamic(this, &UGP3_DragonBossIslandBurnAttack::PauseDamageTick);
		}
		
		/*GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, ActivePlatform->GetFName().ToString());

		ActivePlatform->OnPlayerEnterIsland.RemoveDynamic(this, &UGP3_DragonBossIslandBurnAttack::PauseDamageTick);
		ActivePlatform->OnPlayerExitIsland.RemoveDynamic(this, &UGP3_DragonBossIslandBurnAttack::PauseDamageTick);*/
	}

	IsAlive = false; 

	//Player->Collision->OnComponentBeginOverlap.RemoveDynamic(this, &UGP3_DragonBossIslandBurnAttack::SetActivePlatform);
}

//Checked every frame and once the attack starts -> will return out of functions if something is null or missing
bool UGP3_DragonBossIslandBurnAttack::FailCheck()
{
	if (Platforms.IsEmpty())
		return false;
	if (Player == nullptr)
		return false;
	if (LifetimeTimer <= 0.f && IsAlive)
	{	
		OnAttackFinished(); 		
		return false; 
	}
	return true;
}