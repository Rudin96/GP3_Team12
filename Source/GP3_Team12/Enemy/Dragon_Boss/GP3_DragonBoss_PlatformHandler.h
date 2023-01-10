#pragma once

#include "GameFramework/Actor.h"

#include "GP3_DragonBoss_PlatformHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandleBeginOverlapDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandleEndOverlapDelegate); 

class USphereComponent; 

UCLASS()
class AGP3_DragonBossPlatformHandler : public AActor {

	GENERATED_BODY()

public: 

	AGP3_DragonBossPlatformHandler(); 

	void AdjustRotationTowards(const FVector& Pos);

	void FlipRotation();

protected:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void UpdateBossMovement(float DeltaTime);

private: 

	UFUNCTION()
	void FireBeginOverlapEvents(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void FireEndOverlapEvents(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FQuat NewRotation;

	FVector NewPosition;

public: 

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere; 

	UPROPERTY(BlueprintAssignable)
	FOnHandleBeginOverlapDelegate OnPlayerEnterIsland;

	UPROPERTY(BlueprintAssignable)
	FOnHandleEndOverlapDelegate OnPlayerExitIsland;	

	UPROPERTY(EditAnywhere)
		FVector LocOffset;

	UPROPERTY(EditAnywhere)
		FRotator RotOffset;

	FVector AttachPosition;

	float MaxDistance = 1000.f;

	float DistanceOffset = 1000.f;

	UPROPERTY(EditAnywhere)
		float RotationSpeed = 30.f;

};