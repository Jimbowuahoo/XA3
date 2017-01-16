// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "XA3.h"
#include "GridBlock.h"
#include "UnrealGrid.h"
#include "PaperSpriteComponent.h"

AGridBlock::AGridBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	
	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	
	//setup a dummy sprite
	FString str(TEXT("/Game/Assets/Zombie2_inv_Sprite.Zombie2_inv_Sprite"));
	ConstructorHelpers::FObjectFinderOptional<UPaperSprite> DefaultSprite(*str);
	spriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PrimarySprite"));
	spriteComp->SetSprite(DefaultSprite.Get());

	spriteComp->SetRelativeScale3D(FVector(1.f,1.f, 1.f));
	spriteComp->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	spriteComp->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	spriteComp->SetupAttachment(DummyRoot);


	//initialize location
	location = FVector(0.0, 0.0, 0.0);
}

void AGridBlock::initSprite(int32 spriteNum) {
	FString str = FString::Printf(TEXT("/Game/Assets/Tiles/Sci-Fi-Tiles_A4_Sprite_%d."), spriteNum);
	FString str2 = FString::Printf(TEXT("Sci-Fi-Tiles_A4_Sprite_%d"), spriteNum);
	FString finalStr = str + str2;
	
	
	spriteComp->SetSprite(Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, *finalStr)));
	//spriteComp->SetSprite(Cast<UPaperSprite>(DynamicSprite.Get()));
	spriteComp->SetRelativeScale3D(FVector(8.f, 7.f, 8.f));
	spriteComp->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	
	

}


void AGridBlock::HandleClicked()
{
	/*
	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, OrangeMaterial);
	*/
		

		// Tell the Grid
		if (OwningGrid != nullptr)
		{

				OwningGrid->SquareActivate(x, y);


		}
	//}
}



void AGridBlock::Highlight(bool bOn)
{


	if (bOn)
	{
		BlockMesh->SetMaterial(0, OrangeMaterial);
		spriteComp->SetRenderCustomDepth(true);
		spriteComp->CustomDepthStencilValue = (uint8) EStencilColour::SC_Blue;
	}
	else
	{
		spriteComp->SetRenderCustomDepth(false);
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void AGridBlock::DeathEvent()
{


	OwningGrid->GridModel->setOccupied(x, y, false);
}