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

private: 

	UFUNCTION()
	void FireBeginOverlapEvents(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void FireEndOverlapEvents(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public: 

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere; 

	UPROPERTY(BlueprintAssignable)
	FOnHandleBeginOverlapDelegate OnPlayerEnterIsland;

	UPROPERTY(BlueprintAssignable)
	FOnHandleEndOverlapDelegate OnPlayerExitIsland;	

};