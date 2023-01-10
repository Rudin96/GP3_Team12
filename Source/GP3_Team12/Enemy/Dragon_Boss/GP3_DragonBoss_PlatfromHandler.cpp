#include "GP3_DragonBoss_PlatformHandler.h"
#include "Components/SphereComponent.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"

AGP3_DragonBossPlatformHandler::AGP3_DragonBossPlatformHandler()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere")); 
	RootComponent = Sphere; 

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGP3_DragonBossPlatformHandler::FireBeginOverlapEvents);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AGP3_DragonBossPlatformHandler::FireEndOverlapEvents);
}

void AGP3_DragonBossPlatformHandler::AdjustRotationTowards(const FVector& Pos)
{
	NewRotation = Pos.GetSafeNormal2D().ToOrientationQuat();
	/*float NewRotOffset = RotationSpeed < 0.f ? -15.f : 15.f;
	NewRotation = FQuat::MakeFromEuler(FVector(NewRotation.Euler().X, NewRotation.Euler().Y, NewRotation.Z + 15.f));*/
}

void AGP3_DragonBossPlatformHandler::FlipRotation()
{
	RotationSpeed = RotationSpeed * -1.f;
}

void AGP3_DragonBossPlatformHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBossMovement(DeltaTime);
}

void AGP3_DragonBossPlatformHandler::BeginPlay()
{
	Super::BeginPlay();

	NewRotation = FQuat::MakeFromRotator(RotOffset);
}

void AGP3_DragonBossPlatformHandler::UpdateBossMovement(float DeltaTime)
{
	NewRotation = FQuat::MakeFromEuler(FVector(NewRotation.Euler().X, NewRotation.Euler().Y, NewRotation.Euler().Z + RotationSpeed * DeltaTime));

	NewPosition = NewRotation * LocOffset + GetActorLocation();

	AttachPosition = NewPosition;

	DrawDebugLine(GetWorld(), GetActorLocation(), NewPosition, FColor::Magenta, false, 0.f, (uint8)0U, 5.f);
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