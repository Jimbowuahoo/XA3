// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Actor.h"
#include "GridBlock.generated.h"

class AUnrealGrid;
class AUnit2D;
/*! \class AGridBlock
\brief Class for clickable blocks

A class that defines clickable blocks
*/
UCLASS()
class XA3_API AGridBlock : public AActor
{
	GENERATED_BODY()

	
	//! A public USceneComponent variable.
	/*!
	Dummy root component.
	*/
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	//! A public UStaticMeshComponent variable.
	/*!
	 StaticMesh component for the clickable block 
	*/
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

public:
	/* Constructor
	* Initializes the Grid block object
	*/
	AGridBlock();

	//! A normal member
	/*!
	Initialize the sprite that belongs to this block
	*/
	void initSprite(int32 spriteNum);

	UPROPERTY(EditAnywhere, Category = Sprite)
		//! A public variable.
		/*!
		Sprite class for the Unit.
		*/
	class UPaperSpriteComponent* spriteComp;

	//! A public variable.
	/*!
	Are we currently active?
	*/
	bool bIsActive;

	//! A public variable.
	/*!
	Vector representing the location of the block in the 3D worldspace
	*/
	FVector location;

	//! A public variable.
	/*!
	Pointer to white material used on the focused block
	*/
	UPROPERTY()
	class UMaterial* BaseMaterial;

	//! A public variable.
	/*!
	Pointer to blue material used on inactive blocks
	*/
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	//! A public variable.
	/*!
	Pointer to orange material used on active blocks
	*/
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;

	//! A public variable.
	/*!
	Grid that owns us
	*/
	UPROPERTY()
	class AUnrealGrid* OwningGrid;

	//! A public variable.
	/*!
	X-co-ordinate
	*/
	UPROPERTY()
	uint32 x;
	
	//! A public variable.
	/*!
	Y-co-ordinate
	*/
	
	UPROPERTY()
	uint32 y;

	//! A public variable
	/*!
	Holds the unit that currently occupying this space
	*/
	AUnit2D* occupyingActor;
	


	//! A public member.
	/*!
	Handle Clicks on grid.
	*/
	void HandleClicked();

	//! A public member taking in a boolean value.
	/*!
	Highlight the grid
	*/
	void Highlight(bool bOn);

	//! A public member.
	/*!
	Handles what happens when a unit dies on the grid
	*/
	void DeathEvent();

public:
	//! A public member taking.
	/*!
	Returns DummyRoot subobject.
	*/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	//! A public member.
	/*!
	Returns BlockMesh subobject.
	*/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};



