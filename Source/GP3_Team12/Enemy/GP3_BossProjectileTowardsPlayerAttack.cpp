#include "GP3_BossProjectileTowardsPlayerAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GP3_Team12/Character/GP3_CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"

void UGP3_BossProjectileTowardsPlayerAttack::BeginPlay()
{
	Super::BeginPlay(); 

	Player = Cast<AGP3_CharacterBase>
		(UGameplayStatics::GetPlayerControllerFromID(this, 0)->GetPawn());
}

void UGP3_BossProjectileTowardsPlayerAttack::Attack_Implementation()
{
	if (!FailCheck())
		return; 

	FVector targetLocation = Player->GetActorLocation();
	FVector currentLocation = GetOwner()->GetActorLocation();

	FVector deltaLocation = targetLocation - currentLocation; 
	
	float angleToPlayer = FMath::RadiansToDegrees(atan2(deltaLocation.Y, deltaLocation.X));
	float angleDelta = AngleOfAttack / (ProjectileAmount - 1);
	float currentAngle = angleToPlayer - AngleOfAttack / 2;
	

	for (size_t i = 0; i < ProjectileAmount; i++)
	{
		FVector spawnLocation = FVector(
			currentLocation.X + StartRadius * cos(FMath::DegreesToRadians(currentAngle)),
			currentLocation.Y + StartRadius * sin(FMath::DegreesToRadians(currentAngle)),
			currentLocation.Z
		);

		FVector dir = targetLocation - spawnLocation; 
		dir.Normalize(); 

		FRotator rot = FRotationMatrix::MakeFromX(dir).Rotator(); 
		rot.Yaw -= 90;
				
		AActor* projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, spawnLocation, rot);		

		if (projectile != nullptr && projectile->FindComponentByClass<USkeletalMeshComponent>())
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Green, TEXT("Swoosh"));

			USkeletalMeshComponent* SkelComp = projectile->FindComponentByClass<USkeletalMeshComponent>();

			SkelComp->SetSimulatePhysics(true);
			SkelComp->SetEnableGravity(false); 
			SkelComp->SetAllPhysicsLinearVelocity(dir * ProjectileVelocity);
		}		
		else if(projectile != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Green, TEXT("Primitive Swoosh"));
			UPrimitiveComponent* primitiveComp = projectile->FindComponentByClass<UPrimitiveComponent>();

			primitiveComp->SetAngularDamping(0);
			primitiveComp->SetLinearDamping(0);
			primitiveComp->SetSimulatePhysics(true);
			primitiveComp->SetPhysicsLinearVelocity(dir * ProjectileVelocity);
		}
	
		currentAngle += angleDelta;
	}

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Green, TEXT("Attack Successful"));
}

bool UGP3_BossProjectileTowardsPlayerAttack::FailCheck()
{
	if (Player == nullptr) 		
		return false;
	
	if (ProjectileClass == nullptr)	
		return false;
	
	return true; 
}