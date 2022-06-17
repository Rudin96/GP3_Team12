// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_Boss_AICharacter.h"
#include "../../Game/GP3_GameModeBase.h"
#include "GP3_Team12/Game/GP3_GameInstance.h"

// Sets default values
ABossAICharacter::ABossAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UGP3_GameInstance* GameInstance = UGP3_GameInstance::Get(GetWorld());
	Health = GameInstance->CalculateBossHealthFromStage();
	MaxHealth = GameInstance->CalculateBossHealthFromStage();

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 4.f, FColor::Black, FString::SanitizeFloat(MaxHealth));
}

// Called every frame
void ABossAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossAICharacter::OnPendingKill_Implementation()
{

}

// Called to bind functionality to input
void ABossAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABossAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsPendingKill)
		return 0.f;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AController* Damager = EventInstigator;
	Health -= DamageAmount;
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Cyan, EventInstigator->GetFName().ToString());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Cyan, TEXT("Taking damage from: ") + DamageCauser->GetName());
	if((Health - DamageAmount) < 0)
	{
		if (AGP3_GameModeBase* GameMode = Cast<AGP3_GameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->WinGame();
		}
		GetWorldTimerManager().SetTimer(KillHandle, this, &ABossAICharacter::Kill, KillDelay, false);
		OnPendingKill();
		bIsPendingKill = true;
		return Health - DamageAmount;
	}
	return DamageAmount;
}

void ABossAICharacter::Kill()
{
	Destroy();
}

ABossAICharacter* ABossAICharacter::Get(UObject* WorldContext)
{
	// UWorld* World = WorldContext->GetWorld();
	// TArray<AActor*> Actors = World->PersistentLevel->Actors;
	// return Cast<ABossAICharacter>(Actors[0]);
	TArray<AActor*> Bosses;
	UGameplayStatics::GetAllActorsOfClass(WorldContext->GetWorld(), ABossAICharacter::StaticClass(), Bosses);
	if(Bosses.Num() > 0 && Cast<ABossAICharacter>(Bosses[0]) != nullptr)
	{
		return Cast<ABossAICharacter>(Bosses[0]);
	}
	else
	{
		return nullptr;
	}
}