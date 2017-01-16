// Fill out your copyright notice in the Description page of Project Settings.

#include "XA3.h"
#include "UnrealGrid.h"
#include <stdlib.h>
#include <iostream>
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformProcess.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"
// Sets default values
AUnrealGrid::AUnrealGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	//initialize cells
	cells = TArray<AGridBlock*>();

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Length = 20;
	Height  = 20;
	BlockSpacing = 250.f;

	//Generate grid model and initialize pathing array
	
	GridModel = TSharedPtr<Grid>(new Grid((uint16)Length, (uint16)Height));
	GridPath = TSharedPtr<GridPathing>(new GridPathing(GridModel));

	//no active square selected yet
	activeSquare = false;
	int32 activeX = -1;
	int32 activeY = -1;

	//MaxStep defaults to 10
	maxStep = 10;

	bIsMoving = false;

	squareIndex = 0;

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
}



// Called when the game starts or when spawned
void AUnrealGrid::BeginPlay()
{
	Super::BeginPlay();


	// Number of blocks
	const int32 NumBlocks = Length*Height;

	// Loop to spawn each block
	for (int32 BlockIndex = 0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex / Length) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex % Length) * BlockSpacing; // Modulo gives remainder

																// Make postion vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AGridBlock* NewBlock = GetWorld()->SpawnActor<AGridBlock>(BlockLocation, FRotator(0, 0, 0));


		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
			(NewBlock->x) = BlockIndex% Length;
			(NewBlock->y) = BlockIndex / Length;
			NewBlock->initSprite(GridModel->getTerrain(NewBlock->x, NewBlock->y));
			NewBlock->location += FVector(BlockLocation);
			cells.Add(NewBlock);
			
			std::cout << "";
		}
	}
	for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		AUnit2D *current = *ActorItr;
		snap(current);
		GridModel->setOccupied((uint16) current->getPosX(), (uint16) current->getPosY(), true);
		cells[GridPath->calcIndex((uint16)current->getPosX(), (uint16)current->getPosY())]->occupyingActor = current;
	}


	
}

void AUnrealGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Call parent class tick function  
	
	if (bIsMoving == true && squareIndex >=0)
	{

		FVector destination = cells[GridPath->calcIndex(GridPath->finalPath[squareIndex].getX(), GridPath->finalPath[squareIndex].getY())]->location;
		FVector current = activeActor->GetActorLocation();
		float distance = sqrt(pow((destination[0]- current[0]),2)+ pow((destination[1] - current[1]), 2));
		if (distance < 10.0) {

			squareIndex -= 1;
			if (squareIndex < 0 ) {
				GridModel->setOccupied(GridPath->finalPath[0].getX(), GridPath->finalPath[0].getY(),true);
				cells[GridPath->calcIndex((uint16)activeActor->getPosX(), (uint16)activeActor->getPosY())]->occupyingActor = NULL;
				activeActor->setPosX(GridPath->finalPath[0].getX());
				activeActor->setPosY(GridPath->finalPath[0].getY());
				cells[GridPath->calcIndex((uint16)activeActor->getPosX(), (uint16)activeActor->getPosY())]->occupyingActor = activeActor;
				bIsMoving = false;
				EnableAllInputs();
				activeActor->SetActive(true);
				ClearHighlight();
				return;
			}
			destination = cells[GridPath->calcIndex(GridPath->finalPath[squareIndex].getX(), GridPath->finalPath[squareIndex].getY())]->location ;
			distance = FVector::Dist(destination, activeActor->GetActorLocation() + FVector(0.0, 0.0, 90));
			current = activeActor->GetActorLocation();
			movePerFrame = ((destination)-(current))*(6.0 / distance);
			movePerFrame = movePerFrame - FVector(0.0, 0.0, movePerFrame[2]);
			return;
		}

		FVector next = current + movePerFrame ;
		activeActor->SetActorLocation(next,false);
		

	}
	

}

void AUnrealGrid::SquareActivate(uint32 x, uint32 y)
{
	
	
	if (!activeSquare) {
		ClearHighlight();
		if (GridModel->isOccupied((uint16)x, (uint16)y)) {
			activeActor = cells[GridPath->calcIndex((uint16)x, (uint16)y)]->occupyingActor;
			activeX = x;
			activeY = y;
			GridPath->reset();

			GridPath->explorer(x, y, 0, FMath::Min(20,activeActor->getTU()));

			for (int i = 0; i < (GridPath->pathingArray.Num()); i++)
			{
				if (GridPath->pathingArray[i].getScore() < (uint16)32767) {
					cells[i]->Highlight(true);
				}
			}
			activeSquare = true;

			if (activeActor->getTU() < 20) {
				activeSquare = false;
				ClearHighlight();
			}
		}
		
	}
	else {
		ClearHighlight();
		GridPath->synthesis((uint16)activeX, (uint16)activeY, (uint16)x, (uint16)y);

	
		squareIndex = GridPath->finalPath.Num()-2;
		if (squareIndex >= 0 && activeActor->getTU()>=20) {
			ShowPath(GridPath->finalPath);
			activeActor->setTU(activeActor->getTU() - 20);
			activeActor->setPercentTU();
			GridModel->setOccupied(GridPath->finalPath[squareIndex + 1].getX(), GridPath->finalPath[squareIndex + 1].getY(), false);
			FVector dest = cells[GridPath->calcIndex(GridPath->finalPath[squareIndex].getX(), GridPath->finalPath[squareIndex].getY())]->location;
			float dist = FVector::Dist(dest, activeActor->GetActorLocation() + FVector(0.0, 0.0, 90.0));
			FVector curr = activeActor->GetActorLocation();
			movePerFrame = ((dest)-(curr))*(6.0 / dist);
			movePerFrame = movePerFrame - FVector(0.0,0.0,movePerFrame[2]);
			DisableAllInputs();
			bIsMoving = true;
		}
		
		activeSquare = false;
		
		
	}
}
void AUnrealGrid::Highlight(uint32 x, uint32 y)
{
	cells[GridPath->calcIndex((uint16)x, (uint16)y)]->Highlight(true);
}
void AUnrealGrid::Dim(uint32 x, uint32 y)
{
	cells[GridPath->calcIndex((uint16)x, (uint16)y)]->Highlight(false);
}
void AUnrealGrid::ShowPath(TArray<GridPathingCell> path)
{
	for (int i = 0; i < path.Num(); i++) {
		Highlight(path[i].getX(),path[i].getY());
	}
}
void AUnrealGrid::SetMaxStep(uint32 mStep)
{
	maxStep = mStep;
}
void AUnrealGrid::ClearHighlight()
{
	for (int i = 0; i < cells.Num();i++) {
		cells[i]->Highlight(false);
	}
}
FVector AUnrealGrid::getLocation(uint32 x, uint32 y)
{
	uint16 tempX = (uint16)x;
	uint16 tempY = (uint16)y;
	uint16 index = GridPath->calcIndex(tempX, tempY);
	return cells[index]->location;
}
void AUnrealGrid::snap(AUnit2D * actor)
{

		actor->SetActorLocation(this->getLocation(actor->getPosX(), actor->getPosY()) + FVector(0.0, 0.0, 120.0));
}

void AUnrealGrid::DisableAllInputs()
{
	for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ActorItr->Disable();
	}
}

void AUnrealGrid::EnableAllInputs()
{
	for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ActorItr->Enable();
	}
}
AUnrealGrid::~AUnrealGrid()
{
	
	/*
	delete &GridModel;
	delete &GridPath;
	*/
}
// Called every frame
#undef LOCTEXT_NAMESPACE

