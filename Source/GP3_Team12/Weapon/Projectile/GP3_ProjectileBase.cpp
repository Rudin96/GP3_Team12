// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_ProjectileBase.h"

// Sets default values
AGP3_ProjectileBase::AGP3_ProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AGP3_ProjectileBase::HandleOverlap);

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	RootComponent = Collision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(Collision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AGP3_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnPosition = GetActorLocation();
	GetWorldTimerManager().SetTimer(LifetimeHandle, this, &AGP3_ProjectileBase::DestroySelf, Lifetime);
}

// Called every frame
void AGP3_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ProjectilePathCurveVertical && ProjectilePathCurveHorizontal)
	{
		ApplyCurveOffset();
	}
}

void AGP3_ProjectileBase::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	OnProjectileHit();

	if (AGP3_CharacterBase* Player = Cast<AGP3_CharacterBase>(OtherActor))
	{
	}
	if (ABossAICharacter* BossCharacter = Cast<ABossAICharacter>(OtherActor))
	{
		OtherActor->TakeDamage(Damage, FDamageEvent(), nullptr, this);
	}

	Destroy();
}

void AGP3_ProjectileBase::OnProjectileHit_Implementation()
{
}

void AGP3_ProjectileBase::DestroySelf()
{
	Destroy();
}

void AGP3_ProjectileBase::SetSpeed(const float NewSpeed) const
{
	if (ProjectileMovement != nullptr)
	{
		ProjectileMovement->InitialSpeed = NewSpeed;
		ProjectileMovement->MaxSpeed = NewSpeed;
	}
}

void AGP3_ProjectileBase::SetDamage(const float NewDamage)
{
	Damage = NewDamage;
}

void AGP3_ProjectileBase::ApplyCurveOffset()
{
	const float Current = GetWorldTimerManager().GetTimerElapsed(LifetimeHandle);
	FVector NextPosition = SpawnPosition + GetActorForwardVector() * FVector::DotProduct(GetActorForwardVector(),
		GetActorLocation() - SpawnPosition) + GetActorRightVector() * ProjectilePathCurveHorizontal->GetFloatValue(
		Current / Lifetime);
	NextPosition.Z = SpawnPosition.Z + ProjectilePathCurveVertical->GetFloatValue(Current / Lifetime);
	SetActorLocation(NextPosition);
}
