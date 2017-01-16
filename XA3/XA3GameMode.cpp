// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "XA3.h"
#include "XA3GameMode.h"
#include "XA3PlayerController.h"
#include "Unit2D.h"
#include "UnrealGrid.h"

AXA3GameMode::AXA3GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AXA3PlayerController::StaticClass();
	//DefaultPawnClass = AUnit2D::StaticClass();
	// set default pawn class to our Blueprinted character

}

void AXA3GameMode::switchSides()
{
	for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		if (ActorItr->getCurrentTurn() == ActorItr->getSide())
		{
			ActorItr->resetTU();
			ActorItr->checkFreakout();
			ActorItr->setPercentTU();
			ActorItr->onEndFocus(true);
		}
		//if the current turn belongs to player one, change it to player 2
		if (ActorItr->getCurrentTurn())
		{
			ActorItr->setCurrentTurn(false);
			UE_LOG(LogTemp, Warning, TEXT("P2 Turn"));
		}
		else
		{
			ActorItr->setCurrentTurn(true);
			UE_LOG(LogTemp, Warning, TEXT("P1 Turn"));
		}
	}
	for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		//if the current turn belongs to player one, change it to player 2
		if (ActorItr->getSide() == ActorItr -> getCurrentTurn())
		{
			AXA3PlayerController *controller = Cast<AXA3PlayerController>(GetWorld()->GetFirstPlayerController());
			ActorItr->onBeginFocus();
			ActorItr->SetActive(true);
			controller->Possess(*ActorItr);
			UE_LOG(LogTemp, Warning, TEXT("Got Actor!"));
			break;
		}
	}
	for (TActorIterator<AUnrealGrid> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->ClearHighlight();
	}


}