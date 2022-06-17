#include "GP3_Boss_ProjectileLaunchAttack.h"
#include "GP3_Boss_AttackFunctions.h"

#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

UGP3_BossProjectileLaunchAttack::UGP3_BossProjectileLaunchAttack() {

}

void UGP3_BossProjectileLaunchAttack::BeginPlay()
{
	Super::BeginPlay(); 

	//Get BossAttackFunctions component (contains a couple of functions that are used in most boss-attack-components)
	BossAttackFunctions = GetOwner()->FindComponentByClass<UGP3_BossAttackFunctions>(); 

	//If BossAttackFunctions (UActorComponent) isn't found.. Add an instance of it
	if (BossAttackFunctions == nullptr)
		BossAttackFunctions = Cast<UGP3_BossAttackFunctions>(GetOwner()->AddComponentByClass(UGP3_BossAttackFunctions::StaticClass(), true, FTransform(), true));

	//Check for a scene component with the tag "ProjectileLaunchAttack" - Will use this USceneComponent as projectile start location
	AActor* owner = GetOwner(); 
	ProjectileStartLocation = BossAttackFunctions->GetSceneComponentThroughTag(owner, FName(TEXT("ProjectileLaunchAttack")));
}

void UGP3_BossProjectileLaunchAttack::Attack_Implementation()
{
	if (!FailCheck())
		return;

	FVector currentLocation; 	

	//Set currentPos to the custom USceneComponent of the actor if there is one. Otherwise use actors own location
	if (ProjectileStartLocation != nullptr)
		currentLocation = ProjectileStartLocation->GetComponentLocation();
	else
		currentLocation = GetOwner()->GetActorLocation(); 
	
	//Divides the angle of attack with the ProjectileAmount making for an even spread
	float deltaAngle = AngleOfAttack / ProjectileAmount;
	float currentAngle = 0;

	for (size_t i = 0; i < ProjectileAmount; i++)
	{
		FVector targetPos = FVector(cos(FMath::DegreesToRadians(currentAngle)), sin(FMath::DegreesToRadians(currentAngle)), 0);
		targetPos *= Radius;
		targetPos += currentLocation;

		FVector velocity = BossAttackFunctions->CalculateParabolaVelocity(currentLocation, targetPos, ProjectileLaunchHeight);

		currentAngle += deltaAngle;

		FVector dir = velocity.GetSafeNormal2D(); 
		FRotator rot = FRotationMatrix::MakeFromX(dir).Rotator(); 
		rot.Yaw -= 90; 

		//Spawn actor and add velocity
		AActor* projectile = nullptr; 

		projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, currentLocation, rot);
		if (projectile->FindComponentByClass<USkeletalMeshComponent>())
		{
			USkeletalMeshComponent* SkelComp = projectile->FindComponentByClass<USkeletalMeshComponent>();

			SkelComp->SetSimulatePhysics(true);
			SkelComp->SetEnableGravity(true);
			SkelComp->SetAllPhysicsLinearVelocity(velocity);
		}
		else
		{
			UPrimitiveComponent* primitiveComp = projectile->FindComponentByClass<UPrimitiveComponent>();

			primitiveComp->SetAngularDamping(0);
			primitiveComp->SetLinearDamping(0);
			primitiveComp->SetSimulatePhysics(true);
			primitiveComp->SetPhysicsLinearVelocity(velocity);
		}		
	}	
}

bool UGP3_BossProjectileLaunchAttack::FailCheck()
{
	if (BossAttackFunctions == nullptr)
		return false;
	if (ProjectileClass == nullptr)
		return false;

	return true;
}