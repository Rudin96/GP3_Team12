#include "GP3_Boss_AttackFunctions.h"

#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

UGP3_BossAttackFunctions::UGP3_BossAttackFunctions()
{

}

USceneComponent* UGP3_BossAttackFunctions::GetSceneComponentThroughTag(AActor*& Actor, FName Tag)
{
	USceneComponent* component = nullptr; 

	TArray<UActorComponent*> foundComponents;
	foundComponents = Actor->GetComponentsByTag(USceneComponent::StaticClass(), Tag);

	if (foundComponents.Num() > 0)
	component = Cast<USceneComponent>(foundComponents[0]);

	return component; 
}

bool UGP3_BossAttackFunctions::GroundCheck(FVector StartPos, FVector TargetPos)
{
	FHitResult hit;
	if (GetWorld()->LineTraceSingleByChannel(hit, StartPos, TargetPos, ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam))
		return true;

	return false;
}

FVector UGP3_BossAttackFunctions::CalculateParabolaVelocity(FVector StartPos, FVector Target, float Amplitude)
{
	//Gravity hardcoded - Didn't find a way to get it directly from project settings :P (-980.f)

	float displacementZ = Target.Z - StartPos.Z;
	FVector displacementXY = FVector(Target.X - StartPos.X, Target.Y - StartPos.Y, 0);
	float time = sqrt(-2 * Amplitude / -980.f) + sqrt(2 * (displacementZ - Amplitude) / -980.f);
	FVector velocityZ = FVector::UpVector * sqrt(-2 * -980.f * Amplitude);
	FVector velocityXY = displacementXY / time;

	return velocityXY + velocityZ;
}