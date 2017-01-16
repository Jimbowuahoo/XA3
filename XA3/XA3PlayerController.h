// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "XA3PlayerController.generated.h"

/*! \class AXA3PlayerController
\brief Class for the Controller

A class that contains relevant functions to controll the play character.
*/
UCLASS()
class AXA3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AXA3PlayerController();

	//! Normal Member taking a constand FVector
	/*!
	/param DestLocation vector for destination
	Navigate player to the given world location.
	*/
	void SetNewMoveDestination(const FVector DestLocation);


	//! Normal Member
	/*!
	Input handlers for SetDestination action.
	*/
	void OnSetDestinationPressed();
	//! Normal Member
	/*!
	Input handlers for SetDestination action.
	*/
	void OnSetDestinationReleased();

	//! Normal Member
	/*!
	Disables player input
	*/
	void Disable();
	//! Normal Member
	/*!
	Enables player input
	*/
	void Enable();

	//! Normal Member
	/*!
	Check if the victory condition has been met
	*/
	int32 CheckVictory();

protected:
	//! A protected variable.
	/*!
	boolean for whether the controlled character should navigate to the mouse cursor.
	*/
	uint32 bMoveToMouseCursor : 1;

	//! Inherited Member taking in a float
	/*!
	Tick method for refresh.
	*/
	virtual void PlayerTick(float DeltaTime) override;

	//! Inherited Member
	/*!
	Set up inputs.
	*/
	virtual void SetupInputComponent() override;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	//! Normal Member
	/*!
	Navigate player to the current mouse cursor location.
	*/
	void MoveToMouseCursor();

	//! Normal Member
	/*!
	Navigate player to the current touch location.
	*/
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);


};


