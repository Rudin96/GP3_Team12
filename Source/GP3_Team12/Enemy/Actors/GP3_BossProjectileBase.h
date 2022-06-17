#pragma once

#include "GameFramework/Actor.h"

#include "GP3_BossProjectileBase.generated.h"

class USkeletalMeshComponent;
class USphereComponent; 

UCLASS()
class AGP3_BossProjectileBase : public AActor {

	GENERATED_BODY()

public:

	AGP3_BossProjectileBase(); 

	UFUNCTION()
	void BeginPlay() override; 

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private: 

	void DestroyActor(); 

public: 

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkelMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere; 

	UPROPERTY(EditAnywhere, Category = "Projectile_Settings")
	float Damage = 1;

	UPROPERTY(EditAnywhere, Category = "Projectile_Settings")
	float Lifetime = 5; 

private:

	FTimerHandle timerHandle; 
};