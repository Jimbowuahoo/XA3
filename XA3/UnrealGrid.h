// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XA3.h"
#include "GridBlock.h"
#include "Unit2D.h"
#include "Grid.h"
#include "GridPathing.h"
#include "GridPathingCell.h"
#include "GameFramework/Actor.h"
#include "UnrealGrid.generated.h"

class AUnit2D;
/*! \class AUnrealGrid
\brief Class for the grid, interfacing with Unreal

*/
UCLASS()
class XA3_API AUnrealGrid : public AActor
{
	GENERATED_BODY()

	//! A public USceneComponent variable.
	/*!
	Dummy root component.
	*/
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	//! A public USceneComponent variable.
	/*!
	Text Component for the score.
	*/
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* ScoreText;
	
public:	
	AUnrealGrid();

	//! A public variable.
	/*!
	has a square been selected
	*/
	bool activeSquare;
	//! A public variable.
	/*!
	ActiveSquare x coordinate.
	*/
	int32 activeX;

	//! A public variable.
	/*!
	ActiveSquare y coordinate.
	*/
	int32 activeY;

	//! A public variable.
	/*!
	Maximum number of steps that can be taken.
	*/
	uint32 maxStep;

	bool bIsMoving;
	AUnit2D* activeActor;
	int32 squareIndex;

	FVector movePerFrame;
	
	//! A public variable.
	/*!
	TArray containing all GridBlocks.
	*/
	TArray<AGridBlock*> cells;

	//! A public variable.
	/*!
	Number of blocks along length of grid.
	*/
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 Length;
	//! A public variable.
	/*!
	Number of blocks along height of grid
	*/
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 Height;


	//! A public variable.
	/*!
	Spacing of blocks.
	*/
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		float BlockSpacing;

	//! A public variable.
	/*!
	Model grid.
	*/
	TSharedPtr<Grid> GridModel;
	//! A public variable.
	/*!
	Pathing Grid.
	*/
	TSharedPtr<GridPathing> GridPath;

	//! An inherited member.
	/*!
	Begin AActor Interface.
	*/
	virtual void BeginPlay() override;
	// End AActor interface

	//! A public member taking in two variable.
	/*!
	/param x x coordinate of the cell block
	/param y y coordinate of the cell block
	Handles the block being clicked.
	*/
	void SquareActivate(uint32 x, uint32 y);

	//! A public member taking in two variable.
	/*!
	/param x x coordinate of the cell block
	/param y y coordinate of the cell block
	Hightlights the block on the x y coordinate.
	*/
	void Highlight(uint32 x, uint32 y);

	//! A public member taking in two variable.
	/*!
	/param x x coordinate of the cell block
	/param y y coordinate of the cell block
	Dims the block on the x y coordinate.
	*/
	void Dim(uint32 x, uint32 y);

	//! A public member taking in a variable.
	/*!
	/param path array that contains the path, in cells
	Highlights a Path.
	*/
	void ShowPath(TArray<GridPathingCell> path);

	//! A public member taking in a variable.
	/*!
	/param mStep the number to set the maxStep to
	Sets maxStep variable.
	*/
	void SetMaxStep(uint32 mStep);

	//! A public member.
	/*!
	Clears all highlights.
	*/
	void ClearHighlight();

	//! A public member.
	/*!
	/param x co-ordinate in grid
	/param y co-ordinate in grid
	returns a FVector representation of the center of a cell.
	*/
	FVector getLocation(uint32 x,uint32 y);

	void Tick(float DeltaTime);

	//! A normal member
	/*!
	snap actor to grid position
	*/
	void snap(AUnit2D* actor);

	//! A normal member
	/*!
	Disable all inputs to all units
	*/
	void DisableAllInputs();
	//! A normal member
	/*!
	Enable inputs again
	*/
	void EnableAllInputs();

	~AUnrealGrid();
public:
	//! A public member.
	/*!
	Returns DummyRoot subobject.
	*/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	//! A public member.
	/*!
	Returns ScoreText subobject.
	*/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }
	
};
