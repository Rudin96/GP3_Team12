#include "GP3_CharacterMovementBase.h"
#include "Math/Vector.h"

// Sets default values for this component's properties
UGP3_CharacterMovementBase::UGP3_CharacterMovementBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...

	IConsoleManager::Get().RegisterConsoleVariable(TEXT("character.movementspeed"), MaxMovementSpeed, TEXT("Sets the Players Movementspeed"), ECVF_SetByCode);
	IConsoleManager::Get().RegisterConsoleVariable(TEXT("character.candash"), MaxMovementSpeed, TEXT("Can player Dash?"), ECVF_SetByCode);
	IConsoleManager::Get().RegisterConsoleVariable(TEXT("character.canjump"), MaxMovementSpeed, TEXT("Can Player Jump?"), ECVF_SetByCode);
	IConsoleManager::Get().RegisterConsoleVariable(TEXT("character.jumpheight"), MaxMovementSpeed, TEXT("Sets Players Jump Height"), ECVF_SetByCode);
}


// Called when the game starts
void UGP3_CharacterMovementBase::BeginPlay()
{
	Super::BeginPlay();

	DashCount = MaxDashCount;

	//GetCharacter()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UGP3_CharacterMovementBase::RechargeDashCooldowns, DashRechargeRate, true, DashRechargeRate);
}

// Called every frame
void UGP3_CharacterMovementBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMovement(DeltaTime);

	ApplyDeceleration(DeltaTime);

	ApplyGravity(DeltaTime);

	Velocity = Velocity.GetClampedToMaxSize2D(MaxMovementSpeed);

	//CheckConsoleVariables();

	if (bWantsToDash)
		PerformDash();
}

void UGP3_CharacterMovementBase::UpdateMovement(float DeltaSeconds)
{
	if (!bCanMove)
		return;

	float RemainingTime = GetWorld()->GetDeltaSeconds();
	int Iterations = 0;

	switch (MovementMode)
	{
	case MOVEMENT_NONE: 
		break;
	case MOVEMENT_WALKING: 
		Velocity += (GetCharacter()->GetActorForwardVector() * InputVector.X + GetCharacter()->GetActorRightVector() * InputVector.Y) * Acceleration * DeltaSeconds;
		break;
	case MOVEMENT_FALLING:
		Velocity += (GetCharacter()->GetActorForwardVector() * InputVector.X + GetCharacter()->GetActorRightVector() * InputVector.Y) * Acceleration * AirControl * DeltaSeconds;
		break;
	default:
		break;
	}

	while(RemainingTime > 0 && ++Iterations < 10)
	{
		FHitResult HitRes;
		GetCharacter()->AddActorWorldOffset(Velocity * RemainingTime, true, &HitRes, ETeleportType::None);
		FVector Start = GetCharacter()->GetActorLocation() + FVector::DownVector * GetCharacter()->Collision->GetScaledCapsuleHalfHeight();
		FVector End = Start + FVector::DownVector * GroundedCheckDistance;
		if (HitRes.bBlockingHit)
		{
			if (HitRes.bStartPenetrating)
			{
				GetCharacter()->AddActorWorldOffset(HitRes.Normal * (HitRes.PenetrationDepth + DepenThreshold));
			}
			else
			{
				if (HitRes.ImpactNormal.Z > WalkableFloorZ)
				{
					OnLanded();
					//Calculate walkable angle here
					float DotProduct = FVector::DotProduct(FVector::UpVector, HitRes.ImpactNormal);
					float AngleRadians = FMath::Acos(DotProduct);
					float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
					/*if (AngleDegrees > MaxWalkableAngle)
					{
						MovementMode = MOVEMENT_FALLING;
						break;
					}*/
					
				}

				float CollisionHeight = HitRes.ImpactPoint.Z - Start.Z;
				
				if (CollisionHeight < StepupHeight)
				{
					GetCharacter()->AddActorWorldOffset(FVector(0.f, 0.f, 1.f));
					RemainingTime -= RemainingTime * HitRes.Time;
				}
				else
				{
					Velocity = FVector::VectorPlaneProject(Velocity, HitRes.Normal);
					RemainingTime -= RemainingTime * HitRes.Time;
				}

			}
		}
		else
		{
			if (bWantsToJump && bHasJumped)
			{
				break;
			}
			if (MovementMode == MOVEMENT_FALLING)
			{
				break;
			}
			FHitResult StepDownRes;
			GetWorld()->LineTraceSingleByChannel(StepDownRes, Start, End, ECC_Visibility);
			if (StepDownRes.bBlockingHit)
			{
				float CharacterGroundDist = FVector::Distance(StepDownRes.ImpactPoint, Start);
				if (CharacterGroundDist < GroundedCheckThreshold)
				{
					return;
				}
				if (CharacterGroundDist > GroundedCheckThreshold)
				{
					GetCharacter()->AddActorWorldOffset(FVector(0.f, 0.f, -10.f), true);
					//GetCharacter()->SetActorLocation(HitRes.Location + HitRes.ImpactNormal * 2.f, true);
				}
			}
			else
			{
				MovementMode = MOVEMENT_FALLING;
				break;
			}
			break;
		}
	}
	
}

void UGP3_CharacterMovementBase::BeginDash()
{
	if(DashCount > 0 && !bIsDashing)
	{
		DashStart = GetCharacter()->GetActorLocation();

		if (InputVector.Length() == 0.f)
		{
			DashEnd = DashStart + GetCharacter()->GetActorForwardVector() * DashLength;
		}
		else
		{
			FVector DesiredInputDir = GetCharacter()->GetActorForwardVector() * InputVector.X + GetCharacter()->GetActorRightVector() * InputVector.Y;
			DashEnd = DashStart + DesiredInputDir.GetSafeNormal2D() * DashLength;
		}

		bWantsToDash = true;
		GetCharacter()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UGP3_CharacterMovementBase::RechargeDashCooldowns, DashRechargeRate, true, DashRechargeRate);
		OnBeginDash.Broadcast();
		if(DashCount > 0)
		DashCount--;
	}
}

void UGP3_CharacterMovementBase::PerformDash()
{
	bIsDashing = true;

	MovementMode = MOVEMENT_DASHING;

	FRichCurve DashCurveInfo;
	DashCurveInfo = DashCurve->FloatCurve;

	CurrentDashTime += GetWorld()->GetDeltaSeconds();

	FHitResult HitRes;
	GetCharacter()->SetActorLocation(FMath::Lerp<FVector, float>(DashStart, DashEnd, DashCurve->GetFloatValue(CurrentDashTime)), true, &HitRes, ETeleportType::TeleportPhysics);
	if(HitRes.bBlockingHit)
	{
		if(HitRes.ImpactNormal.Z < .95f)
		{
			bWantsToDash = false;
			bIsDashing = false;
			MovementMode = MOVEMENT_FALLING;
			CurrentDashTime = 0.f;
			if(DashCount > 0)
			DashCount--;

			OnStopDash.Broadcast();
			return;
		}
	}

	if(CurrentDashTime >= DashCurveInfo.GetLastKey().Time)
	{
		bWantsToDash = false;
		CurrentDashTime = 0.f;
		bIsDashing = false;
		MovementMode = MOVEMENT_FALLING;
		OnStopDash.Broadcast();
	}
}

void UGP3_CharacterMovementBase::ApplyGravity(float DeltaSeconds)
{
	switch(MovementMode)
	{
		case MOVEMENT_NONE:
		break;
		case MOVEMENT_WALKING:
		break;
		case MOVEMENT_FALLING:
		if(!bIsDashing)
		Velocity -= FVector::DownVector * GetWorld()->GetDefaultGravityZ() * GravityScale * GetWorld()->GetDeltaSeconds();
		default:
		break;
	}
}


void UGP3_CharacterMovementBase::ApplyDeceleration(float DeltaSeconds)
{
	if (InputVector.Length() == 0.f)
	{
		if (Velocity.IsNearlyZero(20.0))
		{
			Velocity = FVector(0.f, 0.f, Velocity.Z);
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, TEXT("Nearly zero"));
			return;
		}
		else
		{
			Velocity -= Velocity.GetSafeNormal2D() * Deceleration * GetWorld()->GetDeltaSeconds();
		}
	}
	
}

void UGP3_CharacterMovementBase::AddForce(const FVector& Dir)
{
	switch(MovementMode)
	{
		case MOVEMENT_NONE:
		break;
		case MOVEMENT_WALKING:
			Velocity += (Dir * Acceleration) * GetWorld()->GetDeltaSeconds();
		break;
		case MOVEMENT_FALLING:
			Velocity += (Dir * Acceleration) * AirControl * GetWorld()->GetDeltaSeconds();
		break;
	}
}

void UGP3_CharacterMovementBase::AddImpulse(FVector Dir)
{
	Velocity += Dir;
}

void UGP3_CharacterMovementBase::OnLanded()
{
	if(MOVEMENT_FALLING)
	{
		MovementMode = MOVEMENT_WALKING;
		Velocity = FVector(Velocity.X, Velocity.Y, 0.f);
		CurrentJumpCount = 0;
		bWantsToJump = false;
		OnLandedDelegate.Broadcast();
	}
}

void UGP3_CharacterMovementBase::Jump()
{
	bWantsToJump = true;
	if(CurrentJumpCount < MaxJumpCount && MovementMode == MOVEMENT_WALKING)
	{
		AddImpulse(FVector::UpVector * JumpStrength);
		bHasJumped = true;
		MovementMode = MOVEMENT_FALLING;
		CurrentJumpCount++;
	}
}

void UGP3_CharacterMovementBase::StopJump()
{
	bWantsToJump = false;
}

void UGP3_CharacterMovementBase::RechargeDashCooldowns()
{
	if(DashCount < MaxDashCount)
	DashCount++;

	if (DashCount >= MaxDashCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}

void UGP3_CharacterMovementBase::CheckConsoleVariables()
{
	static const auto CVarMoveSpeed = IConsoleManager::Get().FindConsoleVariable(TEXT("character.movementspeed"));
	float BufferMoveSpeed = CVarMoveSpeed->GetFloat();

	if (BufferMoveSpeed != MaxMovementSpeed)
	{
		MaxMovementSpeed = BufferMoveSpeed;
	}
}