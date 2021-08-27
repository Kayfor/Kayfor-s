// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"


// Create enumeration with running states
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DispalyName = "Normal"),
	EMS_Sprinting UMETA(DIspalyName = "Sparinting"),
	EMS_Dead UMETA(DIspalyName = "Dead"),

	EMS_MAX UMETA(DIspalyName = "DefaultMAX")
};

// Create enumeration with  states for StaminaBar
UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DispalyName = "Normal"),
	ESS_BelowMinimum UMETA(DispalyName = "BelowMinimum"),
	ESS_Exhausted UMETA(DispalyName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DispalyName = "ExhaustedRecovering"),

	ESS_MAX UMETA(DIspalyName = "DefaultMAX")
};



UCLASS()
class PROJECTTWO_PRACTICE_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TSubclassOf<class AItemStorage> WeaponStorage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bHasCombatTarget;

	FORCEINLINE void	SetHasCombatTarget(bool HasTarget)
	{
		bHasCombatTarget = HasTarget;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
	FVector CombatTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController * MainPlayerController;


	TArray<FVector> PickupLocations; // Container for handle pickups coordinates
	
	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) 
	{ 
		StaminaStatus = Status;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float StaminaDrainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MinSprintStamina;

	// adjust  player position for make attack process esier
	float InterpSpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemy * CombatTarget;

	FORCEINLINE void SetCombatTarget(AEnemy * Target) 
	{
		CombatTarget = Target;
	}

	/** Set MovementStatus and running speed*/
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	bool bShiftKeyDown;	
	
	// Press Down to enable sprinting
	void ShiftKeyDown();

	// Released to stop sprinting
	void ShiftKeyUp();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CastSpell")
	bool bCoinsPositive;

	/* *
	/* Camera Boom positioning the camera behind the player
	/*
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,  meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/**  Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rates to scale turning functions to the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/* Functionality for change SpringArm length
	*/
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float TargetArmValue_Min;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float TargetArmValue_Max;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float  TargetArmValue_Shift;

	/* *
	/*  Player Stats
	/*
	*/

	   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;
	   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins; // Yes in UE we used full type for int - int32 - it's because of crossplatform scheme.

	   virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	   static float ApplyDamage(AActor * DamagedActor, float BaseDamage, AController * EventInstigator, AActor * DamageCauser, TSubclassOf<class UDamageType> DamageTypeClass);

	   
	   void DecrementHealth(float Amount);
	   void DoT(float Amount);

	   void IncrementHealth(float Amount);
	   void RestoreHealth(float Amount);
	 
	   void Die();

	   virtual void Jump() override;
	   
	   void IncrementCoins(int32 Amount);

	   // Create HoT / DoT functionality;
	   // Make delay between heal flushes
	   FTimerHandle HealTimer;
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
		   float HealTickTime;
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
		   float HealCapacity;
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	   float HoTValue;

	   void DelayBetweenHeal();

	

	   //Create simmilar functionality for DoT
	   // Make delay between damage ticks
	   FTimerHandle DoTTimer;
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT")
		   float DoTTickTime;
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT")
		   float DoTCapacity;
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT")
		   float DoTValue;

	     void DelayForDoT();


	   /** Pay attantion that You MUST use 'class UParticleSystem' here! Not 'class UParticleSystemComponent' !!! */
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoT | Particles")
		   class UParticleSystem * HoTParticles;

	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoT | Sound")
		   class USoundCue * HoTSound;

	   /** Pay attantion that You MUST use 'class UParticleSystem' here! Not 'class UParticleSystemComponent' !!! */
	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT | Particles")
		   class UParticleSystem * DoTParticles;

	   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT | Sound")
		   class USoundCue * DoTSound;	   


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Method which i made for spawning progectiles
	UFUNCTION(BlueprintCallable)
	void SpawnObject(FVector SpawnObjLocation, FRotator SpawnObjRotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backwards input */
	void MoveForward(float value);

	/** Called for side to side input */
	void MoveRight(float value);

	/** Called for Yaw rotation*/
	void TurnUp(float value);

	/** Called for Pitch rotation*/
	void LookUp(float value);

	/** Bool values which fix sprinting on one place*/

	bool bMovingForward;
	bool bMovingRight;

	bool CanMove(float value);

	/** Called via input to turn at a given rate
	* @param Rate is a normilized rate, i.e. 1.0 means 100% of desired turn rate
	*/

	void TurnAtRate(float Rate);

	/** Called via input to look up/down at a given rate
	* @param Rate is a normilized rate, i.e. 1.0 means 100% of desired look up/down rate
	*/
	void LookUpAtRate(float Rate);

	// key for interraction/attack
	bool bEDown;
	void EKeyUp();
	void EKeyDown();

	// key for exit to Menu
	bool bESCDown;
	void ESCDown();
	void ESCUp();

   // key for spawning  projectiles
	bool bFDown;
	void FDown();
	void FUp();

	// Keys for changing CaneraDistance(SpringArm)
	bool bMScrollUp;
	bool bMScrollDown;
	UFUNCTION(BlueprintCallable)
	void MouseScrollUp();
	UFUNCTION(BlueprintCallable)
	void MouseScrollDown();


	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
		class AWeapon * EquippedWeapon; // pointer to weapon class
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
		class AItem * ActiveOverlappingItem;
	

	void SetEquippedWeapon(AWeapon * WeaponToSet);

	// Property to choose actor for spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<AActor>  ActorToSpawn;
	

	FORCEINLINE AWeapon * GetEquippedWeapon()
	{
		 return EquippedWeapon;
	}

	FORCEINLINE void SetActiveOverlappingItem(AItem * Item)
	{		ActiveOverlappingItem = Item;	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anims")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue * AttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage * CombatMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		class UParticleSystem * HitParticles;

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	 UFUNCTION(BlueprintCallable) // (EditAnywhere, BlueprintReadWrite, Category = "Combat")
	void UpdateCombatTarger();

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemy> EnemyFilter; // Subclass for 'GetOverlappingActors' method to filter all collision objects accept related to AEnemy class

	 void SwitchLevel(FName LevelName);

	 // Save system realisation in Main)
	 UFUNCTION(BlueprintCallable)
	 void Savegame();
	 UFUNCTION(BlueprintCallable)
	 void LoadGame(bool SetPosition);

	 UFUNCTION(BlueprintCallable)
		 void LoadGameNoSwitch();

	 UFUNCTION(BlueprintCallable)
	 void  IncrementTargetArmValue();

	 UFUNCTION(BlueprintCallable)
	 void  DecrementTargetArmValue();
	 
};
