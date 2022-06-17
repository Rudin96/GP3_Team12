#pragma once

#include "GameFramework/Actor.h"

#include "GP3_BurnAOE.generated.h"

class USceneComponent; 
class USphereComponent;

UCLASS()
class AGP3_BurnAOE : public AActor {

	GENERATED_BODY()

public:

	AGP3_BurnAOE(); 

public: 

	UPROPERTY()
	USceneComponent* Root = nullptr;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere = nullptr; 

};