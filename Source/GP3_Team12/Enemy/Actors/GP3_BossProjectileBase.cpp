#include "GP3_BossProjectileBase.h"

#include "Kismet/GameplayStatics.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

#include "GP3_Team12/Character/GP3_CharacterBase.h"
#include "GP3_Team12/AI/BossAI/GP3_Boss_AICharacter.h"


AGP3_BossProjectileBase::AGP3_BossProjectileBase()
{
	SkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = SkelMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	Sphere->SetupAttachment(SkelMesh);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGP3_BossProjectileBase::HandleBeginOverlap);
}

void AGP3_BossProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(timerHandle, this, &AGP3_BossProjectileBase::DestroyActor, Lifetime);
}

void AGP3_BossProjectileBase::DestroyActor()
{
	Destroy(); 
}

void AGP3_BossProjectileBase::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (Cast<AGP3_CharacterBase>(OtherActor))
	{
		AGP3_CharacterBase* player = Cast<AGP3_CharacterBase>(OtherActor);

		ABossAICharacter* boss = ABossAICharacter::Get(GetWorld()); 

		if(boss != nullptr)
			player->TakeDamage(Damage, FDamageEvent(), boss->GetController(), this);
	}
	
	Destroy();
}