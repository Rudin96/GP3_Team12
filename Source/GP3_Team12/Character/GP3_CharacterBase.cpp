// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Weapon/GP3_PlayerWeapon.h"
#include "../Game/GP3_GameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Movement/GP3_CharacterMovementBase.h"
#include "GP3_Team12/Game/GP3_WhatIf_Base.h"

// Sets default values
AGP3_CharacterBase::AGP3_CharacterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetCapsuleHalfHeight(96.f);
	Collision->SetCapsuleRadius(48.f);

	bUseControllerRotationYaw = true;

	RootComponent = Collision;

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	CharacterMesh->SetupAttachment(Collision);
	CharacterMesh->AddRelativeLocation(FVector(0.f, 0.f, -90.f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(Collision);
	CameraBoom->TargetArmLength = 1000.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(CameraBoom);

	NewTargetArmLength = CameraBoom->TargetArmLength;

	MovementComp = CreateDefaultSubobject<UGP3_CharacterMovementBase>(TEXT("Character Movement Component"));
	WeaponComponent = CreateDefaultSubobject<UGP3_PlayerWeapon>(TEXT("Weapon Component"));


	IConsoleManager::Get().RegisterConsoleVariable(TEXT("character.canperformactions"), bCanPerformActions, TEXT("Can Player perform any actions"), ECVF_SetByCode);
}

// Called when the game starts or when spawned
void AGP3_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	UGP3_GameInstance* GameInstance = UGP3_GameInstance::Get(GetWorld());

	if (GameInstance->Stage > 1)
	{
		//MaxHealth = GameInstance->PlayerMaxHealth;
		Health = GameInstance->PlayerHealth;
		Health = FMath::Clamp<float>(Health += 50.f, 0.f, MaxHealth);
		CameraSensitivity = GameInstance->CameraSensitivity;

		GameInstance->ApplyWhatIfs();
	}
}

// Called every frame
void AGP3_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//HandleCameraZoom(DeltaTime);

}

// Called to bind functionality to input
void AGP3_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGP3_CharacterBase::HandleMoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGP3_CharacterBase::HandleMoveForward);

	PlayerInputComponent->BindAxis("LookUp", this, &AGP3_CharacterBase::HandleLookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AGP3_CharacterBase::HandleLookRight);

	PlayerInputComponent->BindAxis("Zoom", this, &AGP3_CharacterBase::HandleZoom);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGP3_CharacterBase::HandleJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AGP3_CharacterBase::StopJump);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AGP3_CharacterBase::HandlePrimaryAction);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &AGP3_CharacterBase::StopPrimaryAction);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AGP3_CharacterBase::HandleSecondaryAction);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AGP3_CharacterBase::HandleRollAction);
}

void AGP3_CharacterBase::HandleLookUp(float Value)
{
	if (!MovementComp->bCanMove)
		return;

	CameraBoom->AddLocalRotation(FRotator(Value * CameraSensitivity, 0.f, 0.f));
	CameraBoom->SetRelativeRotation(FRotator(FMath::ClampAngle(CameraBoom->GetRelativeRotation().Pitch, -90.f, 90.f), 0.f, 0.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Cyan, CameraBoom->GetRelativeRotation().ToString());
}

void AGP3_CharacterBase::HandleLookRight(float Value)
{
	if (!MovementComp->bCanMove)
		return;

	AddControllerYawInput(Value * CameraSensitivity);
}

void AGP3_CharacterBase::HandleMoveForward(float Value)
{
	//MovementComp->AddForce(GetActorForwardVector() * Value);
	MovementComp->InputVector.X = Value;
}

void AGP3_CharacterBase::HandleMoveRight(float Value)
{
	//MovementComp->AddForce(GetActorRightVector() * Value);
	MovementComp->InputVector.Y = Value;
}

void AGP3_CharacterBase::HandleJump()
{
	MovementComp->Jump();
}

void AGP3_CharacterBase::StopJump()
{
	MovementComp->StopJump();
}

void AGP3_CharacterBase::KillPlayer()
{
	OnDeath();
	Destroy();
}

void AGP3_CharacterBase::AddForce(const FVector& Dir)
{
	MovementComp->AddForce(Dir);
}

void AGP3_CharacterBase::AddImpulse(const FVector& Dir)
{
	MovementComp->AddImpulse(Dir);
}


float AGP3_CharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	
	if (bIsInvulnerable)
		return 0.0f;


	UGP3_GameInstance* GameInstance = UGP3_GameInstance::Get(GetWorld());


	if (GameInstance == nullptr)
		return 0.0f;

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("Takes damage!"));
	Health -= DamageAmount;

	if ((Health - DamageAmount) <= 0.f)
	{
		OnDeath();
		Destroy();
		return Health - DamageAmount;
	}


	return Health - DamageAmount;
}

void AGP3_CharacterBase::HandlePrimaryAction()
{
	if(bCanPerformActions && bCanShoot)
		WeaponComponent->StartShooting();
}

void AGP3_CharacterBase::StopPrimaryAction()
{
	WeaponComponent->StopShooting();
}

void AGP3_CharacterBase::HandleSecondaryAction()
{
	if (!bCanPerformActions)
		return;

	MovementComp->BeginDash();
}

void AGP3_CharacterBase::HandleZoom(float Value)
{
	NewTargetArmLength += ZoomAmount * Value;
	NewTargetArmLength = FMath::Clamp<float>(NewTargetArmLength, MinBoomLength, MaxBoomLength);
	CameraBoom->TargetArmLength = FMath::Lerp<float>(CameraBoom->TargetArmLength, NewTargetArmLength, ZoomLerpSpeed * GetWorld()->GetDeltaSeconds());
	CameraBoom->TargetArmLength = FMath::Clamp<float>(CameraBoom->TargetArmLength, MinBoomLength, MaxBoomLength);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, TEXT("Zoom boi!"));
}

void AGP3_CharacterBase::HandleRollAction()
{

}

float AGP3_CharacterBase::GetMovementSpeed()
{
	return MovementComp->GetMovementSpeed();
}

void AGP3_CharacterBase::SetMovementSpeed(float NewSpeed)
{
	MovementComp->MaxMovementSpeed = NewSpeed;
}

FVector AGP3_CharacterBase::GetPlayerVelocity()
{
	return MovementComp->Velocity;
}

void AGP3_CharacterBase::SetCameraSensitivity(float Sensitivity)
{
	CameraSensitivity = Sensitivity;
}
