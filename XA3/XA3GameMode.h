// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "XA3GameMode.generated.h"

/*! \class AXA3GameMode
\brief Class for the Game Mode

A class defining the basic game rules
*/
UCLASS(minimalapi)
class AXA3GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	/* Constructor
	* Initializes the Game mode object.
	*/
	AXA3GameMode();

	//! A public member.
	/*!
	Switch the Current turn
	*/
	UFUNCTION(BlueprintCallable, Category = Player)
	void switchSides();

	//! A public member returning a boolean.
	/*!
	Check if all Tu has been exhausted, returns boolean
	*/
	bool checkEnd();
	//! A public variable.
	/*!
	Is the current side one?
	*/

protected:
	//! A public variable.
	/*!
	Is the current side one?
	*/
	bool isPlayerOne = true;
};







