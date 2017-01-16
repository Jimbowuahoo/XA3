// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ContextMenu.generated.h"

/*! \class UContextMenu
\brief Class for UI

Contains necessary functions for the context menu in the gamescape.
*/
UCLASS()
class XA3_API UContextMenu : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	//! A public variable.
	/*!
	Name of the Widget
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context Widget")
		FString WidgetName;
	
};
