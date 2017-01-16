// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PaperCharacter.h"
#include "Unit2D.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMySignal);

/**
*
*/


/*! \enum EUnitState
\brief Enum for possible Unit2D States
*/
UENUM(BlueprintType)
enum class EUnitState : uint8
{
	US_Idle			UMETA(DisplayName = "Idle"),  /**< enum for Idle Character State */
	US_Moving		UMETA(DisplayName = "Moving"),  /**< enum for Moving Character State */
	US_Shooting		UMETA(DisplayName = "Shooting"), /**< enum for Shooting Chracter State*/
	US_Frag			UMETA(DisplayName = "Frag"), /**< enum for Frag Grenade Chracter State*/
	US_Flash		UMETA(DisplayName = "Flash"), /**< enum for Flash Grenade Chracter State*/
	US_Smoke		UMETA(DisplayName = "Smoke"), /**< enum for Smoke Chracter State*/
	US_Hit			UMETA(DisplayName = "Hit"), /**< enum for Hit Chracter State*/
	US_Dead			UMETA(DisplayName = "Death"), /**< enum for Death Chracter State*/
	US_Shoot		UMETA(DisplayName = "Shoot") /**< enum for Shoot Chracter State*/
};

UENUM(BlueprintType)
enum class EStencilColour : uint8
{
	SC_Green	= 252	UMETA(DisplayName = "Green"),  /**< enum for Green Highlight */
	SC_Blue		= 253	UMETA(DisplayName = "Blue"),  /**< enum for Blue Highlight */
	SC_Red		= 254	UMETA(DisplayName = "Red"), /**< enum for Red Highlight*/
};

/*! \class AUnit2D
\brief Class for Units in the game

An Unreal Paper2D class that defines Units in the game. The class contains Unit Statistics
such as HP, TU, RFL, ACC, and BRV, as well as weapon statistics such as damage and ammo
count. The Unit2D class also inherits unreal functions for characters and contains functions
to handle interactions with other Unit2D objects. Unit2D also handles controls for its own
object.
*/
UCLASS()
class XA3_API AUnit2D : public APaperCharacter
{
	GENERATED_BODY()

public:

	/* Constructor
	* Initializes the Unit2D object
	*/
	AUnit2D();

	AUnit2D(uint32 starter);
	//! Inherited Member
	/*!
	Inherited Begin, called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	//! Inherited Member
	/*!
	character update, called every frame
	*/
	virtual void Tick(float DeltaSeconds) override;

	//! Inherited Member
	/*!
	/param InputComponent Inherited input component
	manage input methods
	*/
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//! A normal member
	/*!
	used for handling left clicks, depending on the current state
	*/
	void HandleLeftClick();

	//! A normal member
	/*!
	method for resetting the palyer state back to idle
	*/
	void cancel();



	UFUNCTION(BlueprintCallable, Category = Player)
	//! A normal member returning a Unit2D object
	/*!
	method for returning a Unit2D object that has been moused over
	*/
	AUnit2D* FindFocusedTarget();

	//! A normal member
	/*!
	getter for player side.
	*/
	bool getSide();
	UFUNCTION(BlueprintCallable, Category = Player)
	//! A normal member returning an integer value
	/*!
	getter for current hp.
	*/
	int32 getHP();
	UFUNCTION(BlueprintCallable, Category = Player)
	//! A normal member returning an integer value
	/*!
	getter for accuracy.
	*/
	int32 getACC();
	UFUNCTION(BlueprintCallable, Category = Player)
	//! A normal member returning an integer value
	/*!
	getter for reflex.
	*/
	int32 getRFL();
	UFUNCTION(BlueprintCallable, Category = Player)
	//! A normal member returning an integer value
	/*!
	getter for current time unit.
	*/
	int32 getTU();
	UFUNCTION(BlueprintCallable, Category = Player)
	//! A normal member returning an integer value
	/*!
	getter for current bravery.
	*/
	int32 getBRV();


	//! A normal member
	/*!
		\param hp new healthpoint to set to

		setter for current healthpoints.
	*/
	void setHP(int32 hp);
	//! A normal member
	/*!
	\param acc new accuracy to set to
	setter for accuracy.
	*/
	void setACC(int32 acc);
	//! A normal member
	/*!
	\param rfl new reflex to set to
	setter for reflexes.
	*/
	void setRFL(int32 rfl);
	//! A normal member
	/*!
	\param tu new time unit to set to
	setter for current time unit.
	*/
	void setTU(int32 tu);
	//! A normal member
	/*!
	\param brv new bravery to set to
	setter for current bravery.
	*/
	void setBRV(int32 brv);



	//! A normal member
	/*!
	move method.
	*/
	void move();

	void hit();

	UFUNCTION(BlueprintCallable, Category = Player)
		//! A normal member
		/*!
		\param state new unit state to set to
		A method to change the current state of the unit.
		*/
		void changeState(EUnitState state);

	//! A normal member
	/*!
	A method for handling the action of throwing a frag grenade.
	*/
	void fragGrenade();
	//! A normal member
	/*!
	A method for handling the action of throwing a smoke grenade.
	*/
	void smokeGrenade();
	//! A normal member
	/*!
	A method for handling the action of throwing a Flashbang grenade.
	*/
	void flashbangGrenade();
	//! A normal member
	/*!
	A method for handling the effects of a flashbang.
	*/
	void flash();

	//! A normal member
	/*!
	Reset frag grenade count back to 2.
	*/
	void resetFrag();
	//! A normal member
	/*!
	Reset smoke grenade count back to 2.
	*/
	void resetSmoke();
	//! A normal member
	/*!
	Reset flashbang grenade count back to 2.
	*/
	void resetFlashbang();

	//! A normal member
	/*!
	A method to handle the shoot action.
	*/
	void shoot();
	UFUNCTION(BlueprintCallable, Category = Player)
	//! A normal member
	/*!
	Reset current bullet count to the clipsize.
	*/
	void reload();

	//! A normal member taking an argument and returning a boolean value
	/*!
		\param target pointer to another Unit2D object
		Determine if an attack will hit the target.
	*/
	bool determineHit(AUnit2D *target);

	UFUNCTION(BlueprintCallable, Category = Player)
	//! A normal member taking an argument
	/*!
		\param damage amount of damage taken
		Take damage, check if unit will die.
	*/
	void takeDamage(int damage);

	UFUNCTION(BlueprintCallable, Category = Player)
		//! A normal member
		/*!
		Set TU to 0.
		*/
		void stun();

	//! A normal member
	/*!
	Set TU back to max.
	*/
	void resetTU();

	//! A normal member
	/*!
	Calculation for setting the percentage health.
	*/
	void setPercentHealth();
	//! A normal member
	/*!
	Calculation for setting the percentage TU.
	*/
	void setPercentTU();
	//! A normal member
	/*!
	Calculation for setting the percentage bravery.
	*/
	void setPercentBRV();

	//! A normal member
	/*!
	Check if the unit freaksout based on bravery score.
	*/
	void checkFreakout();
	//! A normal member
	/*!
	Handle the "run" freakout.
	*/
	void run();
	//! A normal member
	/*!
	Handle the "hunker down" freakout.
	*/
	void hunkerDown();

	//! A public variable.
	/*!
	Enum for the current state of the unit.
	*/
	EUnitState state = EUnitState::US_Idle;



	UPROPERTY(EditAnywhere, Category = Colour)
	EStencilColour colour;



	//! A normal member taking in one variable.
	/*!
	Set current turn.
	*/
	void setCurrentTurn(bool turn);

	//! A normal member.
	/*!
	Get current turn.
	*/
	bool getCurrentTurn();

	//! A normal member.
	/*!
	Update the animation of the unit based on the current state.
	*/
	void UpdateAnimation();
	
	//! Normal Member
	/*!
	What to do when the mouse hovers over a character.
	*/
	void onBeginFocus();

	//! Normal Member
	/*!
	what to do when the mouse stops hovering over a character.
	*/
	void onEndFocus();

	//! Normal Member taking in a boolean input
	/*!
	Override for what to do when the mouse stops hovering over a character.
	*/
	void onEndFocus(bool areSure);

	//! Normal Member
	/*!
	Handle highlighting appropriate characters.
	*/
	void HandleHighlight();

	//! Normal Member taking in a FVector
	/*!
	get distance between this unit and another point.
	*/
	double getDistance(FVector v);

	//! Normal Member
	/*!
	Get unit X position on grid.
	*/
	uint32 getPosX();
	//! Normal Member
	/*!
	Get unit Y position on grid.
	*/
	uint32 getPosY();
	//! Normal Member taking in an integer.
	/*!
	Set unit x position on grid.
	*/
	void setPosX(uint32 x);
	//! Normal Member taking in an integer.
	/*!
	Set unit y position on grid.
	*/
	void setPosY(uint32 y);

	//! Normal Member
	/*!
	Find the clicked grid
	*/
	class AGridBlock* FindFocusedGrid();

	//! Signal Member
	/*!
	Send a signal to unreal when a shoot action occurs.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Signal")
	FMySignal signal;

	//! Normal Member
	/*!
	set the state to idle
	*/
	void setIdle();

	//! Normal Member
	/*!
	kill the character
	*/
	void die();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void SetCameraBoom(USpringArmComponent* CB) { CameraBoom = CB; }

	//! Normal Member
	/*!
	Return the selected actor for the mouse pan.
	*/
	bool GetActive();
	//! Normal Member taking in a boolean value
	/*!
	Set the selected actor for the mouse pan.
	*/
	void SetActive(bool val);
	//! Normal Member
	/*!
	Disable actor inputs.
	*/
	void Disable();
	//! Normal Member
	/*!
	Enable actor inputs.
	*/
	void Enable();

	//! public variable
	/*!
	The menu sprite
	*/
	UFUNCTION(Category = "Character", BlueprintCallable)
	class UTexture2D* getMenuSprite();

	//! public variable
	/*!
	Contains the character portrait
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite)
	class UTexture2D* portrait;

	//! public variable
	/*!
	Contains the gun image
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite)
		class UTexture2D* gun;

	//! public variable
	/*!
	Contains the unit's name
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		FString name = FString(TEXT("Name"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pathing)
	class UAudioComponent *shot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundCue *shotCue;


protected:
	//! protected variable
	/*!
	holds the last focused actor.
	*/
	AUnit2D* lastFocusedActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite)
		//! A protected variable.
		/*!
		Sprite class for the Unit.
		*/
		class UPaperSpriteComponent* spriteComp;
	

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		//! A protected variable.
		/*!
		Sprite Flipbook for the Unit's Idle animation.
		*/
		class UPaperFlipbook* IdleAnimation;

	// The animation to play while walking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		//! A protected variable.
		/*!
		Sprite Flipbook for the Unit's walk animation.
		*/
		class UPaperFlipbook* RunningAnimation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		//! A protected variable.
		/*!
		Sprite Flipbook for the Unit's being hit animation.
		*/
		class UPaperFlipbook* HitAnimation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		//! A protected variable.
		/*!
		Sprite Flipbook for the Unit's Death animation.
		*/
		class UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		//! A protected variable.
		/*!
		Sprite Flipbook for the Unit's shooting animation.
		*/
		class UPaperFlipbook* ShootAnimation;

	UPROPERTY(EditAnywhere, Category = Side)
		//! A protected variable.
		/*!
		Boolean that indicates the side that the unit is on.
		*/
		bool isPlayerOne = true;

	//! A protected variable.
	/*!
	Boolean that indicates the if the unit belongs to the current turn.
	*/
	bool isP1Turn = true;

	UPROPERTY(EditAnywhere, Category = Stats)
		//! A protected variable.
		/*!
		Current health of the Unit, used to determine if the unit has been killed,
		editable in engine.
		*/
		int32 healthPoint = 20;
	UPROPERTY(EditAnywhere, Category = Stats)
		//! A protected variable.
		/*!
		Maximum health of the Unit, editable in engine.
		*/
		int32 maxHealth = 40;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		//! A protected variable.
		/*!
		Percentage health of the unit, based on current health divide by
		maximum health, editable in engine.
		*/
		float percentHealth = healthPoint*1.0 / maxHealth;


	//Time Units
	UPROPERTY(EditAnywhere, Category = Stats)
		//! A protected variable.
		/*!
		Current Time Unit, used to determine what actions the unit can still
		take, editable in engine.
		*/
		int32 currentTimeUnit = 50;
	UPROPERTY(EditAnywhere, Category = Stats)
		//! A protected variable.
		/*!
		Maximum Time Unit, editable in engine.
		*/
		int32 maxTimeUnit = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		//! A protected variable.
		/*!
		Percentage Time Unit of the unit, calculated by dividing the current
		TU by the Maximum TU, editable in engine.
		*/
		float percentTU = currentTimeUnit*1.0 / maxTimeUnit;

	//Accuracy and Reflex

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	//! A protected variable.
	/*!
	Accuracy of the Unit, determines chance to hit, editable in engine.
	*/
	int32 accuracy = 40;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	//! A protected variable.
	/*!
	Reflex of the Unit, determies chance to not be hit, editable in engine.
	*/
	int32 reflex = 55;


	//Bravery
	UPROPERTY(EditAnywhere, Category = Stats)
		//! A protected variable.
		/*!
		Max bravery of the Unit, editable in engine.
		*/
		int32 maxBravery = 65;
	UPROPERTY(EditAnywhere, Category = Stats)
		//! A protected variable.
		/*!
		Current Bravery of the Unit, editable in engine.
		*/
		int32 currentBravery = 65;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		//! A protected variable.
		/*!
		Percentage Bravery of the Unit, calculated by dividing the current bravery
		by the maximum. Used to deternime chance to freak out, editable in engine.
		*/
		float percentBrv = currentBravery*1.0 / maxBravery;

	//ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		//! A protected variable.
		/*!
		Maximum ammo of the Unit, editable in engine.
		*/
		int32 clipSize = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//! A protected variable.
	/*!
	current ammo of the Unit, used to determine if the unit may still
	perform the shoot action, editable in engine.
	*/
	int32 bullet = clipSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//! A protected variable.
	/*!
	Number of Frag granades the unit has, used to determined if the unit
	may perform the frag grenade action, editable in engine.
	*/
	int32 frag = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//! A protected variable.
	/*!
	Number of Flashbang grenades the unit has, used to determined if the unit
	may perform the Flashbang action, editable in engine.
	*/
	int32 flashbang = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//! A protected variable.
	/*!
	Number of Smoke granades the unit has, used to determined if the unit
	may perform the Smoke action, editable in engine.
	*/
	int32 smoke = 2;

	//Weapon Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//! A protected variable.
	/*!
	Weapon damage, used to determine the damage the unit will do to another, 
	editable in engine.
	*/
	int32 weaponDmg = 20;


	UPROPERTY(EditAnywhere, Category = Weapon)
		//! A protected variable.
		/*!
		range falloff of the unit, used to determine accuracy based on the distance
		between this unit and the unit it is attacking, editable in engine.
		*/
		float rangeFalloff;

	UPROPERTY(EditAnywhere, Category = Weapon)
		//! A protected variable.
		/*!
		cost, in TU, of the Unit's weapon, editable in engine.
		*/
		int32 weaponCost = 32;


	//Pathing Properties
	UPROPERTY(EditAnywhere, Category = Pathing)
		//! A protected variable.
		/*!
		X co-ordinate of the unit on the grid, editable in engine.
		*/
		uint32 posX = 0;

	UPROPERTY(EditAnywhere, Category = Pathing)
		//! A protected variable.
		/*!
		Y co-ordinate of the unit on the grid, editable in engine.
		*/
		uint32 posY = 0;

	UPROPERTY(EditAnywhere, Category = Pathing)
		//! A protected variable.
		/*!
		Y co-ordinate of the unit on the grid, editable in engine.
		*/
	class AUnrealGrid* pathingParentGrid = NULL;


	


private:

	//! private variable
	/*!
	Holds the unit camera
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	//! private variable
	/*!
	Holds unit location
	*/
	FVector UnitLoc;
	//! private variable
	/*!
	Max Camera pan location
	*/
	float CameraMax = 4300;
	//! private variable
	/*!
	Min Camera pan location
	*/
	float CameraMin = -610;
	bool Active;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	//! private variable
	/*!
	Mouse input vector
	*/
	FVector2D MovementInput;
	//! private variable
	/*!
	Camera input vector
	*/
	FVector2D CameraInput;
	//! private variable
	/*!
	how much to zoom
	*/
	float ZoomFactor;
	//! private variable
	/*!
	Are we zooming in?
	*/
	bool bZoomingIn;

	//! private normal member taking in a float
	/*!
	moves the camera forward and backwards.
	*/
	void MoveForward(float AxisValue);
	//! private normal member taking in a float
	/*!
	moves the camera left and right.
	*/
	void MoveRight(float AxisValue);
	//! private normal member taking in a float
	/*!
	get the mouse coordinates.
	*/
	void GetMouseCoordinate();

	//! private variable
	/*!
	can the unit be interacted with
	*/
	uint32 bCanInteract : 1;

};
