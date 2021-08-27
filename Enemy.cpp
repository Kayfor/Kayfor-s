// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "MainPlayerController.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Agrosphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combatsphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(100.f);

			// Attaching enemy combat collisions to the proper sockets on the enemy's mesh
	EnemyLeftCombatCollizion = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollizionLeft"));
	EnemyLeftCombatCollizion->SetupAttachment(GetMesh(), FName("EnemyLeftSocket")); // Correct way instead string below.
	//EnemyLeftCombatCollizion->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemyLeftSocket"));

	EnemyRightCombatCollizion = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollizionRight"));
	EnemyRightCombatCollizion->SetupAttachment(GetMesh(), FName("EnemyRightSocket")); // Correct way instead string below.
	//EnemyRightCombatCollizion->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemyRightSocket"));

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	bOverlappingCombatSphere = false;

	Health = 100.f;
	MaxHealth = 100.f;
	Damage = 5.f;

	AttackMinTime = 2.f;
	AttackMaxTime = 4.5f;

	DoTTickTime = 2.f;
	DoTCapacity = 4.f;

	DeathDelay = 5.f;

	bHasValidTarget = false;

	// inreping 
	/*
	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	InterpSpeed = 1.0f;
	InterpTime = 3.0f;

	bInterping = false;
	*/
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// this referense will return controller to AI
	AIController = Cast<AAIController>(GetController());

	// Events binding (registartion) to overlap components
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore); // Prevent colliding wiyh explosives)
	
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	// Event hendlers for combat collisions
	EnemyLeftCombatCollizion->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	EnemyLeftCombatCollizion->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	EnemyRightCombatCollizion->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	EnemyRightCombatCollizion->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	// Setup overlapping particles for proper collisions
	EnemyLeftCombatCollizion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyLeftCombatCollizion->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	EnemyLeftCombatCollizion->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnemyLeftCombatCollizion->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	EnemyRightCombatCollizion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyRightCombatCollizion->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	EnemyRightCombatCollizion->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnemyRightCombatCollizion->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Main Character Camera shouldn't collide with enemy collision
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// interping
	/*

	StartPoint = GetActorLocation();
	EndPoint += StartPoint;

	bInterping = false;

	Distance = (EndPoint - StartPoint).Size();
*/
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
	/*
	if (bInterping )
	{
		//1. Create poin with base coordinates
		FVector CurrentLocation = GetActorLocation();
		//2. Create point which will have an able to move in space from current to target location 
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		//3. Implement previous calculation of floation to our Mesh
		SetActorLocation(Interp);

		// Calculate passed distance between points with Interpolation
		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		// If distanse to point too small - toggle movement dirrection
		if ((Distance - DistanceTraveled) <= 1.f)
		{
			
			//GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVectors(StartPoint, EndPoint);
			ToggleInterping();
		}
	}
*/
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::MoveToTarget(AMain * Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController) // for this reference recognition"AIModule" should be added in file 'ProjectTwo_Practice.Build.cs'
	{
		UE_LOG(LogTemp, Warning, TEXT(" Enemy move to target() "));

		// AAIController::MoveTo is complicated struct) - see documentation :
		// @ https://docs.unrealengine.com/en-US/API/Runtime/AIModule/AAIController/MoveTo/index.html @
		//So just create exemplars for it's parameters:

		FAIMoveRequest MoveRequest;  // Exemlar of structure
		MoveRequest.SetGoalActor(Target); // Set the target for AI
		MoveRequest.SetAcceptanceRadius(10.0f); // Distance between capsuls for triggering collizion events

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
}


// Overlap events for Agro
void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT(" Enemy:: AgroSphereOnOverlap Begins. "));		

	if (OtherActor && Alive())
	{
		AMain * Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT(" Enemy:: AgroSphereOnOverlap End. "));

	if (OtherActor)
	{
		AMain * Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{ 
				bHasValidTarget = false;
				if (Main->CombatTarget == this)
				{
					Main->SetCombatTarget(nullptr);
				}
				Main->SetCombatTarget(this);
				Main->SetHasCombatTarget(false);				
				
				Main->UpdateCombatTarger();
				
				// When overlapping finished we switching enemy movement status... 
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

				// Нужно будет запилить систему  при которой НПЦ возвращается в исходную спаун-точку
				//FVector SpawnPoint = GetRelativeLocation(Enemy);
				//MoveToTarget(); // ... and start chasing the target)
				if (AIController)
				{
					AIController->StopMovement();
				}			

				//ToggleInterping();
			}
		}
		

	}

}

// Overlap events for Combat
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT(" Enemy:: CombatSphereOnOverlap Begins. "));	

	if (OtherActor && Alive())
	{
		AMain * Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{
				bHasValidTarget = true;
				Main->SetCombatTarget(this);
				Main->SetHasCombatTarget(true);
				
				Main->UpdateCombatTarger();

				Combattarget = Main;
				bOverlappingCombatSphere = true;
				//Attack();
				float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime); // Enemy will attack with raddom delay
				GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			}				
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{	
	UE_LOG(LogTemp, Warning, TEXT(" Enemy:: CombatSphereOnOverlap End. "));

	if (OtherActor && OtherComp)
	{
		AMain * Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{ // When overlapping finished we switching enemy movement status... 			

				bOverlappingCombatSphere = false;
				MoveToTarget(Main);
				if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
				{							
								MoveToTarget(Main); // ... and start chasing the target)
								Combattarget = nullptr;

								if (Main->CombatTarget == this)
								{
									Main->SetCombatTarget(nullptr);
									Main->bHasCombatTarget = false;
									Main->UpdateCombatTarger();
								}
								
								if (Main->MainPlayerController)
								{
									USkeletalMeshComponent * MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
									if (MainMesh)
										Main->MainPlayerController->RemoveEnemyHealthBar();
								}
				}
				//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);				
				// reset enemy's combat timer when player unaccessible
				GetWorldTimerManager().ClearTimer(AttackTimer);
			}			
		}
	}
}


// declaration of events for CombatCollizion
void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//AWeapon::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT(" CombatOnOverlap Begin. "));

	if (OtherActor)
	{
		AMain * Main = Cast<AMain>(OtherActor);
		if (Main) // if cast was seccessful
		{
			if (Main->HitParticles)
			{  				
				const USkeletalMeshSocket * TipLeftSocket = GetMesh()->GetSocketByName("TipLeftSocket");
				if (TipLeftSocket)
				{
					FVector SocketLocation = TipLeftSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, GetActorLocation(), FRotator(0.f), true);
				}

				const USkeletalMeshSocket * TipRightSocket = GetMesh()->GetSocketByName("TipRightSocket ");
				if (TipRightSocket)
				{
					FVector SocketLocation = TipRightSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, GetActorLocation(), FRotator(0.f), true);
				}
			} 
			if (HitSound)
			{
				UGameplayStatics::PlaySound2D(this, HitSound);
			}

			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//AWeapon::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT(" CombatOnOverlap End. "));
}


void AEnemy::ActivationCombatCollision()
{
	EnemyLeftCombatCollizion->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnemyRightCombatCollizion->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivationCombatCollision()
{
	EnemyLeftCombatCollizion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyRightCombatCollizion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (Alive() && bHasValidTarget)
	{
		if(AIController )
		{
				AIController->StopMovement();
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if (!bAttacking) // if we do not attaking already
		{
			bAttacking = true;
			UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && CombatMontage)// if it's valid
			{
				int32 Section = FMath::RandRange(0, 2); // Give random number form specific range to play proper attack animation
				switch (Section)
				{
				case 0:
								AnimInstance->Montage_Play(CombatMontage, 1.35f);
								AnimInstance->Montage_JumpToSection(FName("Attack_0"), CombatMontage);
				break;

				case 1:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;

				case 2:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;

				default:
					;  // empty state
				}			
			}
		}
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	if (bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime); // Enemy will attack with raddom delay
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (Health - DamageAmount <= 0.f) // If enemy health <=0 it dies
	{
		Health -= DamageAmount;
		Die(DamageCauser);
	}
	else // if not - it's continuing to take damage
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}
void AEnemy::Die(AActor * Causer)
{
	//UE_LOG(LogTemp, Warning, TEXT(" Enemy health is over... It died!  "));
	UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 2.f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}

	// Update enemy movement status and deactivate it collisions
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	DeactivationCombatCollision();
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	AMain * Main = Cast<AMain>(Causer);
	if (Main)
	{
		Main->UpdateCombatTarger();
	}
}


void AEnemy::DelayForDoT()
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

	DoTCapacity--;  // Hardcoded value 100/10 = 10 tics with 10 damage/tick

	if ((Health - DoTValue) < 0)
	{	
			Die(AEnemy::Combattarget);		
	}
	else
	{
		Health -= DoTValue;
	}
	if (EnemyDoTTimer.IsValid() && DoTCapacity <= 0)  // stop timer when 'capacity' reach specific value 
	{
		GetWorld()->GetTimerManager().ClearTimer(EnemyDoTTimer);
		UE_LOG(LogTemp, Warning, TEXT("DelayBetweenHeal has been stopped! "));
	}
}


void AEnemy::DoT(float Amount)
{
	DoTValue = Amount; // Get value from proper item
	//Main timer which set timer 'DoTTimer' to Current actor(this) and call hendler 'DelayBetweenDamage' each 'DoTTickTimer' value , 'Tick' enabled.
	GetWorld()->GetTimerManager().SetTimer(EnemyDoTTimer, this, &AEnemy::DelayForDoT, DoTTickTime, true);
} 


void AEnemy::DeathEnd()
{
	GetMesh()->bNoSkeletonUpdate = true;
	GetMesh()->bPauseAnims = true;	
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay, true);
}

bool AEnemy::Alive()
{	
	return (GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead);
}

void AEnemy::Disappear()
{
	Destroy();
}

// Interping
/*
void AEnemy::ToggleInterping()
{
	bInterping = !bInterping;
}

void AEnemy::SwapVectors(FVector & VecOne, FVector & VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}
*/


