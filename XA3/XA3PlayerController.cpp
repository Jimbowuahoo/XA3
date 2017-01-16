// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "XA3.h"
#include "XA3PlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Unit2D.h"

AXA3PlayerController::AXA3PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AXA3PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		if (GetPawn() != nullptr)
		{
			MoveToMouseCursor();
		}
	}
}

void AXA3PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AXA3PlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AXA3PlayerController::OnSetDestinationReleased);
	
	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AXA3PlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AXA3PlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AXA3PlayerController::OnResetVR);
}

void AXA3PlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AXA3PlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AUnit2D* MyPawn = Cast<AUnit2D>(GetPawn()))
		{
			
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			AUnit2D* current = Cast<AUnit2D>(this->GetPawn());

			

			if (current->state == EUnitState::US_Idle)
			{
				SetNewMoveDestination(Hit.ImpactPoint);
				
			}
			// We hit something, move there
			
			
		}
	}
}

void AXA3PlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AXA3PlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AXA3PlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AXA3PlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AXA3PlayerController::Disable()
{
	DisableInput(this);
}

void AXA3PlayerController::Enable() {
	EnableInput(this);
}

int32 AXA3PlayerController::CheckVictory()
{

	int32 end = 1;
	for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		//UE_LOG(LogXA3, Warning, TEXT("Side1, %d"), ActorItr->getSide());
		if (1 == ActorItr->getSide())
		{
			end = 0;
		}
	}
	if (end == 1)
	{
		printf("Player 2 Wins");
		UE_LOG(LogXA3, Warning, TEXT("Player 2 Wins"));
		return end;
	}
	end = 2;
	//UE_LOG(LogXA3, Warning, TEXT("Current Side: %d"), getSide());
	for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		//UE_LOG(LogXA3, Warning, TEXT("Side1, %d Side2, %d"), getSide(), ActorItr->getSide());
		if (0 == ActorItr->getSide())
		{
			end = 0;
		}
	}
	if (end == 2)
	{
		printf("Player 1 Wins");
		UE_LOG(LogXA3, Warning, TEXT("Player 1 Wins"));
		return end;
	}
	return end;
}