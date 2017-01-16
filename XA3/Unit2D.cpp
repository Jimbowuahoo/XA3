// Fill out your copyright notice in the Description page of Project Settings.

#include "XA3.h"
#include "Unit2D.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "XA3PlayerController.h"
#include "XA3GameMode.h"
#include "GridBlock.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Grid.h"
#include <time.h>
#include <stdlib.h> 
#include <chrono>


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)


// Sets default values
AUnit2D::AUnit2D()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FString str(TEXT("/Game/Assets/Zombie2_inv_Sprite.Zombie2_inv_Sprite"));
	ConstructorHelpers::FObjectFinderOptional<UPaperSprite> DefaultSprite(*str);
	spriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PrimarySprite"));
	spriteComp->SetSprite(DefaultSprite.Get());
	
	FString str2(TEXT("/Game/Assets/Zombie2_inv.Zombie2_inv"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> defSprite(*str2);
	portrait = defSprite.Object;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	//FVector printout = GetActorLocation();
	//UE_LOG(LogXA3, Warning, TEXT("X%f Y%f Z%f"), printout.X, printout.Y, printout.Z);
	UnitLoc = GetActorLocation() + FVector(0.0f, -1000.0f, 0.0f);
	CameraBoom->SetWorldLocationAndRotationNoPhysics(GetActorLocation() + FVector(0.0f, -1000.0f, 0.0f), FRotator(180.0f, -90.0f, 180.0f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
										  // Create a camera...
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	TopDownCameraComponent->SetOrthoWidth(5000);
	//TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	

	spriteComp->MarkRenderStateDirty();
	spriteComp->bGenerateOverlapEvents = true;
	spriteComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (!isPlayerOne)
	{
		colour = EStencilColour::SC_Red;
	}
	else
	{
		colour = EStencilColour::SC_Green;
	}

	srand(static_cast<unsigned int>(time(0)));

}

AUnit2D::AUnit2D(uint32 Starter)
{
}

// Called when the game starts or when spawned
void AUnit2D::BeginPlay()
{
	SetActorEnableCollision(true);
	Super::BeginPlay();


}

// Called every frame
void AUnit2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAnimation();
	HandleHighlight();
	GetMouseCoordinate();
	//FVector NewLocation2 = TopDownCameraComponent->GetComponentLocation();
	//UE_LOG(LogXA3, Warning, TEXT("X %f, Y%f, Z%f"), NewLocation2.X, NewLocation2.Y, NewLocation2.Z);
	//FVector CurrentLocation = TopDownCameraComponent->GetComponentLocation();
	if (!MovementInput.IsZero() && Active)
	{
		MovementInput = MovementInput * 25.0f;
		FVector NewLocation = TopDownCameraComponent->GetComponentLocation();
		NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
		NewLocation += GetActorUpVector() * MovementInput.Y * DeltaTime;
		if (NewLocation.X < CameraMin)
			NewLocation.X = CameraMin;
		if (NewLocation.X > CameraMax)
			NewLocation.X = CameraMax;
		if (NewLocation.Y < CameraMin)
			NewLocation.Y = CameraMin;
		if (NewLocation.Y > CameraMax)
			NewLocation.Y = CameraMax;
		//UE_LOG(LogXA3, Warning, TEXT("X %f, Y%f, Z%f"), NewLocation.X, NewLocation.Y, NewLocation.Z);
		TopDownCameraComponent->SetWorldLocation(NewLocation);
	}

}

// Called to bind functionality to input
void AUnit2D::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Cancel", IE_Pressed, this, &AUnit2D::cancel);
	InputComponent->BindAction("Left", IE_Released, this, &AUnit2D::HandleLeftClick);
	InputComponent->BindAction("Test", IE_Pressed, this, &AUnit2D::hit);
	InputComponent->BindAction("Reload", IE_Pressed, this, &AUnit2D::reload);
	InputComponent->BindAxis("MoveForward", this, &AUnit2D::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUnit2D::MoveRight);
}



void AUnit2D::HandleLeftClick()
{
	FHitResult Hit(ForceInit);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	AXA3PlayerController *controller = Cast<AXA3PlayerController>(GetWorld()->GetFirstPlayerController());
	if (isP1Turn == isPlayerOne)
	{
		if (Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("got blocking hit"));

			if (Hit.GetActor()->GetClass()->IsChildOf(AUnit2D::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Clicked"));
				if (state == EUnitState::US_Shooting)
				{
					UE_LOG(LogTemp, Warning, TEXT("bang"));
					state = EUnitState::US_Shoot;
					shoot();
				}
				else if (state == EUnitState::US_Frag)
				{
					fragGrenade();
				}
				else if (state == EUnitState::US_Flash)
				{
					flashbangGrenade();
				}
				else if (state == EUnitState::US_Smoke)
				{
					smokeGrenade();
				}
				else if (state == EUnitState::US_Idle)
				{
					UE_LOG(LogTemp, Warning, TEXT("Possesed"));
					if (FindFocusedTarget() != nullptr)
					{
						if (FindFocusedTarget()->getSide() == getCurrentTurn())
						{
							print("Possessed.");
							FindFocusedTarget()->onBeginFocus();
							onEndFocus(true);
							for (TActorIterator<AGridBlock> ActorItr(GetWorld()); ActorItr; ++ActorItr)
							{
								if (FindFocusedTarget() -> getDistance(ActorItr->GetActorLocation()) < 100)
								{
									ActorItr->HandleClicked();
									break;
								}
							}
							if(FindFocusedTarget()->GetClass()->IsChildOf(AUnit2D::StaticClass()))
							{
							Active = false;
							FindFocusedTarget()->Active = true;
							TopDownCameraComponent->SetWorldLocation(GetActorLocation() + GetActorLocation().Z + GetActorUpVector() * -500);
							//FindFocusedTarget()->GetTopDownCameraComponent()->SetWorldLocation(FindFocusedTarget()->GetActorLocation());
							}
							controller->Possess(FindFocusedTarget());
						}

					}
				}

				
			}
			else
			{
				if (state == EUnitState::US_Frag)
				{
					fragGrenade();
				}
				else if (state == EUnitState::US_Flash)
				{
					flashbangGrenade();
				}
				else if (state == EUnitState::US_Smoke)
				{
					smokeGrenade();
				}
				else if (state == EUnitState::US_Idle)
				{
					UE_LOG(LogTemp, Warning, TEXT("got grid"));
					AGridBlock* gridBlock = FindFocusedGrid();
					gridBlock->HandleClicked();
				}
			}
		}


	}
}

bool AUnit2D::getSide()
{
	return isPlayerOne;
}

int32 AUnit2D::getHP()
{
	return healthPoint;
}

int32 AUnit2D::getACC()
{
	return accuracy;
}

int32 AUnit2D::getRFL()
{
	return reflex;
}

int32 AUnit2D::getTU()
{
	return currentTimeUnit;
}

int32 AUnit2D::getBRV()
{
	return maxBravery;
}



void AUnit2D::setHP(int32 hp)
{
	healthPoint = hp;
}

void AUnit2D::setACC(int32 acc)
{
	accuracy = acc;
}

void AUnit2D::setRFL(int32 rfl)
{
	reflex = rfl;
}

void AUnit2D::setTU(int32 tu)
{
	currentTimeUnit = tu;
}

void AUnit2D::setBRV(int32 brv)
{
	currentBravery = brv;
}



void AUnit2D::move()
{
	//move
}

AGridBlock* AUnit2D::FindFocusedGrid()
{
	FHitResult Hit(ForceInit);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit)
	{
		if (Hit.GetActor()->GetClass()->IsChildOf(AGridBlock::StaticClass()))
		{
			AGridBlock* target = Cast<AGridBlock>(Hit.GetActor());
			if (target)
			{
				return target;
			}
		}
	}
	return nullptr;
}

AUnit2D* AUnit2D::FindFocusedTarget()
{
	FHitResult Hit(ForceInit);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit)
	{
		if (Hit.GetActor()->GetClass()->IsChildOf(AUnit2D::StaticClass()))
		{
			AUnit2D* target = Cast<AUnit2D>(Hit.GetActor());
			if (target)
			{
				return target;
			}
		}
	}
	return nullptr;
}

void AUnit2D::hit()
{
	UE_LOG(LogTemp, Warning, TEXT("E Pressed"));
}

void AUnit2D::shoot()
{
	if (state == EUnitState::US_Shoot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shot"));
		if (FindFocusedTarget() != nullptr)
		{
			AUnit2D *target = FindFocusedTarget();
			if (bullet == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Out of Ammo"));
				print("Check Ammo.");
				state = EUnitState::US_Idle;
			}
			else if (currentTimeUnit < weaponCost)
			{
				UE_LOG(LogTemp, Warning, TEXT("No TU"));
				print("No Energy");
				state = EUnitState::US_Idle;
			}
			else if (target->getSide() != isPlayerOne && getDistance(target -> GetActorLocation()) < 1750)
			{

				determineHit(target);
				bullet -= 1;
				UE_LOG(LogTemp, Warning, TEXT("Current Health: %d"), target->getHP());
				if (bullet == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("out of Ammo!"), bullet);
					print("Check Ammo.");
				}
				currentTimeUnit -= weaponCost;
				setPercentTU();
				//state = EUnitState::US_Idle;
			}
			else if (target->getSide() != isPlayerOne)
			{
				state = EUnitState::US_Idle;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Friendly Target"));
				print("That's a Friendly Unit, I hope.");
				state = EUnitState::US_Idle;
			}
		}

	}
}

bool AUnit2D::determineHit(AUnit2D *target)
{
	/* generate secret number between 0 and 100: */
	//shot->Play();
	int random = rand() % 101;

	float chance = (accuracy*1.0f / target->getRFL()) * 100 - 30;

	UE_LOG(LogTemp, Warning, TEXT("Hit Chance: %2f%"), chance);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AUnit2D::setIdle, 0.5, false, 1.0f);
	if (chance > random)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit!"));
		FString start(TEXT("Got em! "));
		FString end(TEXT(" damage done."));
		signal.Broadcast();
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, start + FString::FromInt(weaponDmg) + end);
		target->takeDamage(weaponDmg);
		
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Miss!"));
		print("Miss");
		return false;
	}

}

void AUnit2D::setIdle()
{
	state = EUnitState::US_Idle;
}

void AUnit2D::takeDamage(int damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage Taken"));
	healthPoint -= damage;
	currentBravery -= (rand() %(damage-10) + 5);
	setPercentHealth();
	setPercentBRV();
	FTimerHandle handle;
	FTimerHandle handle2;
	
	if (healthPoint <= 0)
	{
		state = EUnitState::US_Dead;
		print("Oh No! I died D:");
		GetWorld()->GetTimerManager().SetTimer(handle2, this, &AUnit2D::die, 0.5, false, 1.0f);
		for (TActorIterator<AGridBlock> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (getPosX() == ActorItr->x && getPosY() == ActorItr->y) {
				ActorItr->DeathEvent();
			}

		}
		
		
	}
	else
	{
		state = EUnitState::US_Hit;
	}
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AUnit2D::setIdle, 0.5, false, 1.0f);
	AXA3PlayerController *controller = Cast<AXA3PlayerController>(GetWorld()->GetFirstPlayerController());
	controller->CheckVictory();
}

void AUnit2D::die()
{
	Destroy();
}

void AUnit2D::fragGrenade()
{
	if (currentTimeUnit < 35)
	{
		UE_LOG(LogTemp, Warning, TEXT("No TU"));
		print("No TU");
		cancel();
	}
	else if (frag < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Grenades"));
		print("No Grenades");
		cancel();
	}
	else
	{
		//throw frag

		FHitResult Hit(ForceInit);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			if (getDistance(Hit.Location) < 1200)
			{
				for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					//if the current turn belongs to player one, change it to player 2
					if (ActorItr->getSide() != isPlayerOne)
					{
						if (Hit.bBlockingHit)
						{
							if (ActorItr->getDistance(Hit.Location) < 500)
							{
								ActorItr->takeDamage(30);
							}
						}
					}
				}
				print("Frag Thrown");
				currentTimeUnit -= 35;
				setPercentTU();
				frag -= 1;
				cancel();
			}
			else
			{
				print("Not In Distance!");
				cancel();
			}
		}
	}
}

void AUnit2D::smokeGrenade()
{
	if (currentTimeUnit < 35)
	{
		UE_LOG(LogTemp, Warning, TEXT("No TU"));
		print("No TU");
		cancel();
	}
	else if (smoke < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Grenades"));
		print("No Grenades");
		cancel();
	}
	else
	{
		//throw frag

		FHitResult Hit(ForceInit);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			if (getDistance(Hit.Location) < 1200)
			{
				for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					//if the current turn belongs to player one, change it to player 2
					if (ActorItr->getSide() != isPlayerOne)
					{
						if (Hit.bBlockingHit)
						{
							if (ActorItr->getDistance(Hit.Location) < 500)
							{
								ActorItr->setACC(ActorItr->getACC() - 5);
							}
						}
					}
				}
				print("Smoke Thrown!");
				currentTimeUnit -= 35;
				setPercentTU();
				smoke -= 1;
				cancel();
			}
			else
			{
				print("Not In Distance!");
				cancel();
			}
		}
	}
}

void AUnit2D::flashbangGrenade()
{
	if (currentTimeUnit < 35)
	{
		UE_LOG(LogTemp, Warning, TEXT("No TU"));
		print("No TU");
		cancel();
	}
	else if (flashbang < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Grenades"));
		print("No Grenades");
		cancel();
	}
	else
	{
		//throw frag

		FHitResult Hit(ForceInit);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			if (getDistance(Hit.Location) < 1200)
			{
				for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					//if the current turn belongs to player one, change it to player 2
					if (ActorItr->getSide() != isPlayerOne)
					{
						if (Hit.bBlockingHit)
						{
							if (ActorItr->getDistance(Hit.Location) < 500)
							{
								ActorItr->flash();
							}
						}
					}
				}
				print("Flash Thrown!");
				currentTimeUnit -= 35;
				setPercentTU();
				flashbang -= 1;
				cancel();
			}
			else
			{
				print("Not In Distance!");
				cancel();
			}
		}
		//flash all affected


	}

}

void AUnit2D::flash()
{
	currentTimeUnit -= 35;
	setPercentTU();
}

void AUnit2D::stun()
{
	currentTimeUnit = 0;
}

void AUnit2D::resetFrag()
{
	frag = 2;
}

void AUnit2D::resetSmoke()
{
	smoke = 2;
}

void AUnit2D::resetFlashbang()
{
	flashbang = 2;
}

void AUnit2D::reload()
{
	if (currentTimeUnit >= 25)
	{
		currentTimeUnit -= 25;
		setPercentTU();
		UE_LOG(LogTemp, Warning, TEXT("Reloaded"));
		bullet = clipSize;
	}
	
}

void AUnit2D::resetTU()
{
	currentTimeUnit = maxTimeUnit;
}

void AUnit2D::setPercentHealth()
{
	percentHealth = healthPoint*1.0 / maxHealth;
}

void AUnit2D::setPercentTU()
{
	percentTU = currentTimeUnit*1.0 / maxTimeUnit;
}

void AUnit2D::setPercentBRV()
{
	percentBrv = currentBravery*1.0 / maxBravery;
}

void AUnit2D::cancel()
{
	this->state = EUnitState::US_Idle;
}



void AUnit2D::changeState(EUnitState state)
{
	this->state = state;
}

void AUnit2D::checkFreakout()
{
	int randomHundred = rand() % 101;
	int randomAction = rand() % 3;
	int freakoutChance = percentBrv * 100 + 10;

	if (freakoutChance < randomHundred)
	{
		/*
		if (randomAction == 0) // Hunker
		{
			hunkerDown();
		}
		else if (randomAction == 1) // Run
		{
			run();
		}
		else //blind fire
		{

		}
		*/
		hunkerDown();
	}
}

void AUnit2D::run()
{
	//move away
	stun();
}

void AUnit2D::hunkerDown()
{
	//hunker animation?
	stun();
}

void AUnit2D::setCurrentTurn(bool turn)
{
	isP1Turn = turn;
	AXA3PlayerController *controller = Cast<AXA3PlayerController>(GetWorld()->GetFirstPlayerController());
	Active = false;
	controller->UnPossess();
	if (turn)
	{
		UE_LOG(LogTemp, Warning, TEXT("P1"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("P2"));
	}
}

bool AUnit2D::getCurrentTurn()
{
	return isP1Turn;
}

void AUnit2D::UpdateAnimation()
{
	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation;
	if (state == EUnitState::US_Idle)
	{
		DesiredAnimation = IdleAnimation;
	}
	else if (state == EUnitState::US_Shoot)
	{
		DesiredAnimation = ShootAnimation;
	}
	else if (state == EUnitState::US_Dead)
	{
		DesiredAnimation = DeathAnimation;
	}
	else if (state == EUnitState::US_Hit)
	{
		DesiredAnimation = HitAnimation;
	}
	else
	{
		DesiredAnimation = IdleAnimation;
	}

	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}



void AUnit2D::onBeginFocus()
{
	GetSprite()->SetRenderCustomDepth(true);
	GetSprite()->CustomDepthStencilValue = (uint8)colour;
}

void AUnit2D::onEndFocus()
{
	AXA3PlayerController *controller = Cast<AXA3PlayerController>(GetWorld()->GetFirstPlayerController());
	AUnit2D *targetPawn = Cast<AUnit2D>(controller->GetPawn());
	if (targetPawn == this)
	{

	}
	else
	{
		GetSprite()->SetRenderCustomDepth(false);
	}

}

void AUnit2D::onEndFocus(bool areSure)
{
	if (areSure)
	{
		GetSprite()->SetRenderCustomDepth(false);
	}

}

void AUnit2D::HandleHighlight()
{
	AUnit2D* highlighted = FindFocusedTarget();
	if (highlighted)
	{
		if (highlighted != lastFocusedActor)
		{
			if (lastFocusedActor && lastFocusedActor != this) lastFocusedActor->onEndFocus();
			highlighted->onBeginFocus();
			lastFocusedActor = highlighted;
		}
	}
	else
	{
		if (lastFocusedActor && lastFocusedActor != this) lastFocusedActor->onEndFocus();
		lastFocusedActor = nullptr;
	}
}

UTexture2D* AUnit2D::getMenuSprite()
{
	return portrait;
}

double AUnit2D::getDistance(FVector v)
{
	FVector currentLocation = GetActorLocation();

	double distance = sqrt((v.X - currentLocation.X)*(v.X - currentLocation.X) + (v.Y - currentLocation.Y)*(v.Y - currentLocation.Y));
	UE_LOG(LogTemp, Warning, TEXT("Distance to center: %f"), distance);
	return distance;
}

uint32 AUnit2D::getPosX()
{
	return posX;
}

uint32 AUnit2D::getPosY()
{
	return posY;
}
void AUnit2D::setPosX(uint32 x)
{
	posX = x;
	return;
}

void AUnit2D::setPosY(uint32 y)
{
	posY = y;
	return;
}

void AUnit2D::GetMouseCoordinate()
{
	AXA3PlayerController *PlayerController = Cast<AXA3PlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController != NULL && Active)
	{
		float LocationX;
		float LocationY;
		PlayerController->GetMousePosition(LocationX, LocationY);
		FVector2D MousePosition(LocationX, LocationY);
		//UE_LOG(LogXA3, Warning, TEXT("Mouse: X%f, Y%f"), MousePosition.X, MousePosition.Y);
		UGameViewportClient *vpcl = GEngine->GameViewport;
		FVector2D ViewportSize;
		vpcl->GetViewportSize(ViewportSize);
		//FSlateRect Rect = vpcl->GetWindow()->GetFullScreenInfo();
		//FVector2D ViewportSize = Rect.GetSize();
		//UE_LOG(LogXA3, Warning, TEXT("Viewport size: X%f, Y%f"), ViewportSize.X, ViewportSize.Y);
		//FIntPoint ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		float Move, TenthX, TenthY;
		TenthX = ViewportSize.X / 10;
		TenthY = ViewportSize.Y / 10;
		//UE_LOG(LogXA3, Warning, TEXT("Viewport Tenth size: X%f, Y%f"), TenthX, TenthY);
		FVector NewLocation = TopDownCameraComponent->GetComponentLocation();
		NewLocation += GetActorRightVector() * -10;
		if (TopDownCameraComponent->GetComponentLocation().Z < 300)
			TopDownCameraComponent->SetWorldLocation(NewLocation);
		//UE_LOG(LogTemp, Warning, TEXT("UnitLocation %f %f %f"), TopDownCameraComponent->GetComponentLocation().X, TopDownCameraComponent->GetComponentLocation().Y, TopDownCameraComponent->GetComponentLocation().Z);
		if (MousePosition.X > (ViewportSize.X - TenthX) && MousePosition.X > 1 && MousePosition.Y > 1)
		{
			Move = -1 * (MousePosition.X - (ViewportSize.X - TenthX));
			//UE_LOG(LogXA3, Warning, TEXT("MOVE1 %f"), Move);
			if (Move > -200 && Move < 200)
				MoveRight(Move);
		}
		if (MousePosition.X < TenthX && MousePosition.X > 1 && MousePosition.Y > 1)
		{
			Move = TenthX - MousePosition.X;
			//UE_LOG(LogXA3, Warning, TEXT("MOVE2 %f"), Move);
			if (Move > -200 && Move < 200)
				MoveRight(Move);
		}
		if (MousePosition.Y > (ViewportSize.Y - TenthY) && MousePosition.X > 1 && MousePosition.Y > 1 && (MousePosition.X < ViewportSize.X / 4 || MousePosition.X > ViewportSize.X * 3 / 4))
		{
			Move = -1 * (MousePosition.Y - (ViewportSize.Y - TenthY) + 20);
			//UE_LOG(LogXA3, Warning, TEXT("MOVE3 %f"), Move);
			if (Move > -200 && Move < 200)
				MoveForward(Move);
		}
		else if (MousePosition.Y >(ViewportSize.Y - 0.5*TenthY) && MousePosition.X > 1 && MousePosition.Y > 1)
		{
			Move = -1 * (MousePosition.Y - (ViewportSize.Y - TenthY) + 20);
			//UE_LOG(LogXA3, Warning, TEXT("MOVE3 %f"), Move);
			if (Move > -200 && Move < 200)
				MoveForward(Move);
		}
		if (MousePosition.Y < TenthY && MousePosition.X > 1 && MousePosition.Y > 1)
		{
			Move = TenthY - MousePosition.Y + 40;
			//UE_LOG(LogXA3, Warning, TEXT("MOVE4 %f"), Move);
			if (Move > -200 && Move < 200)
				MoveForward(Move);
		}
	}
}

//Input functions
void AUnit2D::MoveForward(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -50.0f, 50.0f);
	//UE_LOG(LogXA3, Warning, TEXT("MovementY %f"), MovementInput.Y);
}

void AUnit2D::MoveRight(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -50.0f, 50.0f);
	//UE_LOG(LogXA3, Warning, TEXT("MovementX %f"), MovementInput.Y);
}

bool AUnit2D::GetActive() { return Active; }

void AUnit2D::SetActive(bool val) { Active = val; }

void AUnit2D::Disable()
{
	DisableInput(NULL);
	TopDownCameraComponent->SetWorldLocation(GetActorLocation() + GetActorLocation().Z + GetActorUpVector() * -500);
	Active = false;
}

void AUnit2D::Enable() {
	EnableInput(NULL);
}
/*
bool AUnit2D::CheckVictory() 
{
	bool end = true;
	UE_LOG(LogXA3, Warning, TEXT("Current Side: %d"), getSide());
	for (TActorIterator<AUnit2D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogXA3, Warning, TEXT("Side1, %d Side2, %d"), getSide(), ActorItr->getSide());
		if (getSide() == ActorItr->getSide())
		{
			end = false;
		}
	}
	UE_LOG(LogXA3, Warning, TEXT("End %d"), end);
	if (end == true)
	{
		if (isP1Turn == true)
		{
			printf("Player 2 Wins");
			UE_LOG(LogXA3, Warning, TEXT("Player 2 Wins"));
		}
		else
		{
			printf("Player 1 Wins");
			UE_LOG(LogXA3, Warning, TEXT("Player 1 Wins"));
		}
	}
	return end;
}
*/

