// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "MainPlayerController.h"
#include "Enemy.h"
#include "FirstSaveGameSystem.h"
#include "ItemStorage.h"
#include "Projectile_Item.h"


// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (pulls towards the player if there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent > (TEXT(" CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.f; // Camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arn based on controller

	// Set size for capsule collizion
	GetCapsuleComponent()->SetCapsuleSize(41.f, 97.f);

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attachet the camera to the end of the boom and let the boom ajust to match
	//the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	// Set turn rates for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	// SpringArm parametrs
	TargetArmValue_Min = 200.f;
	TargetArmValue_Max = 800.f;
	TargetArmValue_Shift = 100.f;

	// Don't rotate when the controller rotates.
	// Let that just affect the camera.
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	// Configure character movement(in dirrection of input)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // add rotation rate
	GetCharacterMovement()->JumpZVelocity = 550.f; // Height of jump
	GetCharacterMovement()->AirControl = 0.2f;

	
	MaxHealth = 100.f;
	Health = 100.f;

	MaxStamina = 150.f;	
	Stamina = 150.f;

	Coins = 0;

	HealTickTime = 3.f;
	HealCapacity = 30.f;

	DoTTickTime = 2.f;
	DoTCapacity = 4.f;	

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	bShiftKeyDown = false;	
	bEDown = false;
	bESCDown = false;
	bFDown = false;

	bMScrollUp = false;
	bMScrollDown = false;

	//Initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;

	bHasCombatTarget = false;

	bMovingForward = false;
    bMovingRight = false;

	bCoinsPositive = false;
}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
	bCoinsPositive = true;
}

void AMain::ShowPickupLocations()
{
	/**  1-st version of debug pickups locations with spheres  /throught indexes

	for(int32 i = 0; i < PickupLocations.Num(); i++)
	{
	UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.f, 12, FLinearColor::Blue, 25.f, 0.5f);
	}
	*/

	/** 2-nd  version of debug pickups locations with spheres / throught objects */
	for (FVector Location : PickupLocations) // here we can use keyword 'auto' insted FVector - compiler will use proper type depends on container type
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 12, FLinearColor::Blue, 25.f, 0.5f);
	}
}

void AMain::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else	
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::ShiftKeyDown()
{
		bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMain::SetEquippedWeapon(AWeapon * WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = WeaponToSet;
}

void AMain::EKeyUp()
{
	bEDown = false;
}

void AMain::EKeyDown() // Interact with weapon using 'E' key
{
	bEDown = true;   // When 'E' key pressed
	UE_LOG(LogTemp, Warning, TEXT(" 'E' button was pressed! "));
	// Check that player is alive
	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	// Check that pause is active 
	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;
	
	if (ActiveOverlappingItem)  // and Main collides with weapon collision
	{
		AWeapon * Weapon = Cast<AWeapon>(ActiveOverlappingItem); // Make this weapon active
		if (Weapon) // if weapon successfully activated
		{
			Weapon->Equip(this);    // equip it
			SetActiveOverlappingItem(nullptr); // and deactivate overlapping event
		}
	}
	else if (EquippedWeapon)
	{
		Attack();
	}
	
}

void AMain::FUp()
{
	bFDown = false;
}

void AMain::FDown() // When 'F' key pressed
{
	bFDown = true;   // change flag
	UE_LOG(LogTemp, Warning, TEXT(" 'F' button was pressed! "));
	// Check that player is alive
	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	// Check that pause is active 
	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;

	if (bCoinsPositive == false) return;  // I desided that each firebal should cost 1 coin
	
		// Call method for spawn projectiles     
		SpawnObject(GetActorLocation()+(GetActorForwardVector()*400), GetActorRotation()); 		
		UE_LOG(LogTemp, Warning, TEXT(" Fireball was spawned! "));

		Coins--;  // Decremet 1 coin when projectile have been spawned
		if (Coins < 1)  // check if some cons left
			bCoinsPositive = false;
	
}

void AMain::MouseScrollUp()
{
	bMScrollUp = true;
	bMScrollDown = false;
	IncrementTargetArmValue();
}

void AMain::MouseScrollDown()
{
	bMScrollUp = false; 
	bMScrollDown = true;
	DecrementTargetArmValue();
}
void AMain::ESCDown()
{
	bESCDown = true;

	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMwnu();
	}
}

void AMain::ESCUp()
{
	bESCDown = false;
}


// Manipulations with Health

void AMain::DecrementHealth(float Amount)
{
	UE_LOG(LogTemp, Warning, TEXT(" Your health was decreased - you was damaged!  "));
	
	Health -= Amount;
	if (Health - Amount <= 0.f)
	{ 
		Health -= Amount; // This line updates 'HealthBar' status
		Die();
	}		
}

void AMain::IncrementHealth(float Amount)
{
	UE_LOG(LogTemp, Warning, TEXT(" Your health was restored! "));

	if (Health + Amount > MaxHealth)
	{
		Amount = MaxHealth - Health; // Fix bug with MaxStamina overflow with heal
		Health += Amount;
	}
	else
	{
		Health += Amount;
	}	
}


void AMain::DelayBetweenHeal() // Method which launches with HealTimer
{
	UE_LOG(LogTemp, Warning, TEXT("HoT Heal is active! "));

	if (HoTParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HoTParticles, GetActorLocation(), FRotator(0.f), false);
	}

	if (HoTSound)
	{
		UGameplayStatics::PlaySound2D(this, HoTSound);
	}

	HealCapacity -= 5.f;  // Hardcoded value 30/5 = 6 tics

	if (Health + HoTValue > MaxHealth)
	{
		HoTValue = MaxHealth - Health; // Fix bug with MaxStamina overflow with heal
		Health += HoTValue;
	}
	else
	{
		Health += HoTValue;
	}

	if (HealTimer.IsValid() && HealCapacity <= 0)  // stop timer when 'capacity' reach specific value 
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimer);
		UE_LOG(LogTemp, Warning, TEXT("DelayBetweenHeal has been stopped! "));			
	}
}

// HoT mechanic realisation
void AMain::RestoreHealth(float Amount)
{
	UE_LOG(LogTemp, Warning, TEXT(" Your health was sligtly restored! "));

	HoTValue = Amount; // Get value from proper item

	//Main timer which set timer 'HealTimer' to Current actor(this) and call hendler 'DelayBetweenHeal' each 'HealTickTimer' value , 'Tick' enabled.
	GetWorld()->GetTimerManager().SetTimer(HealTimer, this, &AMain::DelayBetweenHeal, HealTickTime, true);		
}

void AMain::DelayForDoT()
{
	UE_LOG(LogTemp, Warning, TEXT("DoT is active! You take some damage over time! "));

	if (DoTParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DoTParticles, GetActorLocation(), FRotator(0.f), false);
	}

	if (DoTSound)
	{
		UGameplayStatics::PlaySound2D(this, DoTSound);
	}

	DoTCapacity --;  // Hardcoded value 100/10 = 10 tics with 10 damage/tick

	if ((Health - DoTValue) < 0)
	{
		Die();
	}
	else 
	{
		Health -= DoTValue;
	}
	if (DoTTimer.IsValid() && DoTCapacity <= 0)  // stop timer when 'capacity' reach specific value 
	{
		GetWorld()->GetTimerManager().ClearTimer(DoTTimer);
		UE_LOG(LogTemp, Warning, TEXT("DelayBetweenHeal has been stopped! "));
	}
}

void AMain::DoT(float Amount)
{
	DoTValue = Amount; // Get value from proper item
	//Main timer which set timer 'DoTTimer' to Current actor(this) and call hendler 'DelayBetweenDamage' each 'DoTTickTimer' value , 'Tick' enabled.
	GetWorld()->GetTimerManager().SetTimer(DoTTimer, this, &AMain::DelayForDoT, DoTTickTime, true);
}

float AMain::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	//DecrementHealth(DamageAmount);

	Health -= DamageAmount;
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount; // This line updates 'HealthBar' status
		Die();
		if (DamageCauser)
		{
			AEnemy * Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}
	}

	return DamageAmount;
}

float AMain::ApplyDamage(AActor * DamagedActor, float BaseDamage, AController * EventInstigator, AActor * DamageCauser, TSubclassOf<class UDamageType> DamageTypeClass)
{
	return 0.0f;
}

void AMain::Die()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return; // Check if Character alredy dead it shoudn't play death animation again

	UE_LOG(LogTemp, Warning, TEXT(" Your health is over... You are die!  "));
	UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.35f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
		// по-логике дальше должен вырубаться контроль движений персонажа, затем показываться дэзстэйт-скрин с предложениями типа "Retry" и "Quit to Main Menu"
		// При наличии МэйнМенюхи естественно...

		SetMovementStatus(EMovementStatus::EMS_Dead);
	}
		ESCDown(); // Call pause menu when deth was come.
}

void AMain::Jump()
{
	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
//	UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation() + FVector(0.f, 0.f, 175.f), 25.f, 12, FLinearColor::Blue, 25.f, 0.25f);
	   
	MainPlayerController = Cast<AMainPlayerController>(GetController());

	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (Map != "SunTemple")
	{
				LoadGameNoSwitch();
			// Correct load after load game from other level
			if (MainPlayerController)
			{
				MainPlayerController->GameModeOnly();
			}	
	}

		
}

// defenition for spawning objects such as projectiles
void AMain::SpawnObject(FVector, FRotator)
{
	FActorSpawnParameters SpawnParams;

	// make a pointer to  signature of the object for spawn (in my case type AProjectile_Item, which was made as son-class Projectile_Item of class Item )
	AActor * SpawnActorRef = GetWorld()->SpawnActor<AProjectile_Item>(ActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			UE_LOG(LogTemp, Warning, TEXT("ShiftKeyDown = true! "));
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				if (bMovingForward || bMovingRight)
				{
					Stamina -= DeltaStamina;
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
					UE_LOG(LogTemp, Warning, TEXT(" Sprinting! "));
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
			}
			/*
			if (bMovingForward || bMovingRight)
			{
				
			}		*/		
			
				
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{				
				if (bMovingForward || bMovingRight)
				{
					Stamina -= DeltaStamina;
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}					
				else
					SetMovementStatus(EMovementStatus::EMS_Normal);
			}
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		else  // Shift key up
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;

	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetMovementStatus(EMovementStatus::EMS_Normal);
			Stamina += DeltaStamina;
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}
		else // Shift key up
		{
			SetMovementStatus(EMovementStatus::EMS_Normal);
			Stamina += DeltaStamina;
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
		}

		break;

	default:	;
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

// Called to bind functionality for input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AMain::EKeyDown);
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &AMain::EKeyUp);

	PlayerInputComponent->BindAction("ShootFireball", IE_Pressed, this, &AMain::FDown);
	PlayerInputComponent->BindAction("ShootFireball", IE_Released, this, &AMain::FUp);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::ESCUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMain::TurnUp);
	PlayerInputComponent->BindAxis("LookUp", this, &AMain::LookUp);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::LookUpAtRate);	
}

void AMain::MoveForward(float value)
{
	bMovingForward = false;
	if(CanMove (value))
	{
		
		// found out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
		bMovingForward = true;
		UE_LOG(LogTemp, Warning, TEXT("MoveForward = true! "));
	}
}

void AMain::MoveRight(float value)
{
	bMovingForward = false;
	if (CanMove(value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
		bMovingForward = true;
		UE_LOG(LogTemp, Warning, TEXT("MoveRight = true! "));
	}
}

void AMain::TurnUp(float value)
{
	if (CanMove(value))
		AddControllerYawInput(value);
}

void AMain::LookUp(float value)
{
	if (CanMove(value))
		AddControllerPitchInput(value);
}

bool AMain::CanMove(float value)
{
	if (MainPlayerController)
	{
		return  (value != 0.0f) && (!bAttacking) && 
			(MovementStatus != EMovementStatus::EMS_Dead) &&
			!MainPlayerController->bPauseMenuVisible;
	}
	return false;
}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMain::Attack()  // Activation of combat animation
{
	if (!bAttacking && (MovementStatus != EMovementStatus::EMS_Dead))
	{
		bAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0,  2); // Give random number form specific range to play proper attack animation
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);

				// play 2d sound before attack animation / commented because I do this with blueprin with orientation to time-notifyer in combat montage
				//if (AttackSound)					UGameplayStatics::PlaySound2D(this, AttackSound);
				
				break;

			case 1:
				AnimInstance->Montage_Play(CombatMontage, 1.3f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);

				
				// if (AttackSound)					UGameplayStatics::PlaySound2D(this, AttackSound);
				
				break;

			case 2:
				AnimInstance->Montage_Play(CombatMontage, 1.3f);
				AnimInstance->Montage_JumpToSection(FName("Attack_3"), CombatMontage);


				//if (AttackSound)					UGameplayStatics::PlaySound2D(this, AttackSound);

				break;

			default:
				;  // empty state
			}			
		}		
	}
}

void AMain::AttackEnd()
{
	bAttacking = false;

	if (bEDown)
	{
		Attack();
	}
}

void AMain::PlaySwingSound()
{
	if(EquippedWeapon->SwingSound)
	UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
}

void AMain::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMain::UpdateCombatTarger()
{
	TArray <AActor*> OverlappingActors;  // Create array for GetOverlappingAftors
	GetOverlappingActors(OverlappingActors, EnemyFilter);
	//Closest unit should be pryority target//
	//Check that TArray isn't empty
	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}
	// Create one more arrey and Cast his element/s to proper type 
	AEnemy * ClosestEnemy = Cast<AEnemy> (OverlappingActors[0]);

	if(ClosestEnemy) // if Cast was successful
	{ 
		FVector Loacation = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation().Size() - Loacation.Size());

		for ( auto Actor : OverlappingActors) // Perebor massiva ' OverlappingActors'
		{
			AEnemy * Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation().Size() - Loacation.Size());
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
				
		}
		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}

void AMain::SwitchLevel(FName LevelName)
{	UWorld * World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();

		// We can't initialize FString with FName so we do this with dereference operator) 
		FName CurrentLevelName(*CurrentLevel);

		if (CurrentLevelName != LevelName)
		{
			Savegame();
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
}

void AMain::Savegame()
{
	// Make a pointer of our savegame class to have an access to structure's fields
	UFirstSaveGameSystem * SaveGameInstance =  Cast <UFirstSaveGameSystem>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGameSystem::StaticClass()));

	// Save all values to proper variables
	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Stamina = Stamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Coins = Coins;

	// SaveLevel Logic
	FString MapName = GetWorld()->GetMapName();
	// Remove system pefix for map name for correct saving
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	// Show map name with system prefix in the log
	UE_LOG(LogTemp, Warning, TEXT("MapName: %s"), *MapName)

		SaveGameInstance->CharacterStats.LevelName = MapName;

	//saving the weapon
	if (EquippedWeapon)
	{
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	// Saving player's position
	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	// make a save slot and put the data there
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void AMain::LoadGame(bool SetPosition)
{
	UFirstSaveGameSystem * LoadGameInstance = Cast <UFirstSaveGameSystem>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGameSystem::StaticClass()));

	 LoadGameInstance = Cast <UFirstSaveGameSystem>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	 // Loading players's stats
	 Health= LoadGameInstance->CharacterStats.Health;
	 MaxHealth=	 LoadGameInstance->CharacterStats.MaxHealth;
	 Stamina=	 LoadGameInstance->CharacterStats.Stamina;
	 MaxStamina = LoadGameInstance->CharacterStats.MaxStamina ;
	 Coins = LoadGameInstance->CharacterStats.Coins ;
	

	 // Loading player's weapon
	 if (WeaponStorage)
	 {
		 // Make a pointer to ItemStorage where stored our weapons
		 AItemStorage * Weapons =	GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		 if (Weapons)
		 {
			 // Get Weapon Name from GameInstance
			 FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
			 
			 if (Weapons->WeaponMap.Contains(WeaponName)) //just to be sure that it's != null 
			 {
				 // Get access to proper weapon and equip it
					 AWeapon * WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
					 WeaponToEquip->Equip(this);
			 }			
		 }
	 }
	 
	 // Loading player's position
       if(SetPosition)
	   {
		 SetActorLocation(LoadGameInstance->CharacterStats.Location );
		 SetActorRotation(LoadGameInstance->CharacterStats.Rotation );
	   }

	   SetMovementStatus(EMovementStatus::EMS_Normal);
	   GetMesh()->bPauseAnims = false;
	   GetMesh()->bNoSkeletonUpdate = false;

	   if (LoadGameInstance->CharacterStats.LevelName != TEXT(""))
	   {
		   FName LevelName(*LoadGameInstance->CharacterStats.LevelName);

		   SwitchLevel(LevelName);
	   }
}

void AMain::LoadGameNoSwitch()
{
	UFirstSaveGameSystem * LoadGameInstance = Cast <UFirstSaveGameSystem>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGameSystem::StaticClass()));

	LoadGameInstance = Cast <UFirstSaveGameSystem>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	// Loading players's stats
	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	Coins = LoadGameInstance->CharacterStats.Coins;

	// Loading player's weapon
	if (WeaponStorage)
	{
		// Make a pointer to ItemStorage where stored our weapons
		AItemStorage * Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons)
		{
			// Get Weapon Name from GameInstance
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;

			if (Weapons->WeaponMap.Contains(WeaponName)) //just to be sure that it's != null 
			{
				// Get access to proper weapon and equip it
				AWeapon * WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
				WeaponToEquip->Equip(this);
			}
		}
	}


	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
}

void AMain::IncrementTargetArmValue()
{
	if(CameraBoom->TargetArmLength <= TargetArmValue_Max ) 
		CameraBoom->TargetArmLength  +=  TargetArmValue_Shift;
}

void AMain::DecrementTargetArmValue()
{
	if (CameraBoom->TargetArmLength >= TargetArmValue_Min) 
		CameraBoom->TargetArmLength -=  TargetArmValue_Shift;
}

