// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"	
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"



// Create enumeration with Enemy states
UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle						UMETA(DispalyName = "Idle"),
	EMS_MoveToTarget		UMETA(DIspalyName = "MoveToTarget"),
	EMS_Attacking				UMETA(DispalyName = "Attacking"),
	EMS_Dead						UMETA(DisplayName = "Dead"),

	EMS_MAX						UMETA(DIspalyName = "DefaultMAX")
};

UCLASS()
class PROJECTTWO_PRACTICE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	bool bHasValidTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status)
	{
		EnemyMovementStatus = Status;
	}

	FORCEINLINE EEnemyMovementStatus  GetEnemyMovementStatus()
	{
		return EnemyMovementStatus;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent * AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent * CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController * AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	AMain * Combattarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Particles")
		class UParticleSystem * HitParticles;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Stats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Stats")
	float Damage;

	// Make delay between damage ticks
	FTimerHandle EnemyDoTTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT")
		float DoTTickTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT")
		float DoTCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT")
		float DoTValue;

	void DelayForDoT() ;

   void DoT(float Amount);

	/** Pay attantion that You MUST use 'class UParticleSystem' here! Not 'class UParticleSystemComponent' !!! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT | Particles")
		class UParticleSystem * DoTParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoT | Sound")
		class USoundCue * DoTSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Sound")
		class USoundCue * HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Sound")
		 USoundCue * SwingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI | Combat")
		class UBoxComponent * EnemyRightCombatCollizion;  // to create collizion for combat realisation

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI | Combat")
		UBoxComponent * EnemyLeftCombatCollizion;  // to create collizion for combat realisation

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "AI | Anims")
		class UAnimMontage * CombatMontage;

	
	// Make delay between enemy's attacks
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	float AttackMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	float AttackMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;

	FTimerHandle DeathTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DeathDelay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) ;
	UFUNCTION()
	virtual void  AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;


	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void  CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMain * Target);

	   
	// Overlapping events for enemy combat collision
	UFUNCTION()
		virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void  CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	// Activate weapon collision for playing particles in proper moment of the strike
	UFUNCTION(BlueprintCallable)
		void ActivationCombatCollision();

	// Deactivate weapon collision for stop play particles in proper moment of the strike
	UFUNCTION(BlueprintCallable)
		void DeactivationCombatCollision();
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anims")
		bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
		void AttackEnd();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void Die(AActor * Causer);

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UFUNCTION(BlueprintCallable)
	bool Alive();

	void Disappear();

	// Functions for realisation returning to the start point when enemy leave certain radius or stop attack Main (interping)

		//Vector for 'StartPont'
	UPROPERTY(EditAnywhere)
		FVector StartPoint;

	//Vector for 'EndPont'
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "True"))
		FVector EndPoint;

	// Platform Interpolation speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		float InterpSpeed;

	// Platform Interpolation transition time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		float InterpTime;

	// Timer for cycling interpolation between points
	//FTimerHandle InterpTimer;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		bool bInterping;

	float Distance;

	void ToggleInterping();
	void SwapVectors(FVector & VecOne, FVector & VecTwo);
	*/

};
