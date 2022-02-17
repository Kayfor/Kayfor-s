// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponState : uint8
{	
	EWS_Pickup			 UMETA(DisplayName = "Pickup"),
	EWS_Equipped	   	 UMETA(DispalyName = "Equipped"),

	EWS_MAX				 UMETA(DisplayName = "DefaultMax")
};

/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// constructor
	AWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponState WeaponState;  // to set states equiped/pickup

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	bool bWeaponParticle; // to make additional effects

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue * OnEquipSound; // to play sound after equip

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sceletal Mesh")
	class USkeletalMeshComponent * SkeletalMesh; // basicly to equip)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
		class UBoxComponent * CombatCollizion;  // to create collizion for combat realisation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		 USoundCue * SwingSound; // to play sound after equip

protected:

	virtual void BeginPlay() override;

public:

	// copy overlap events from base class and override them	 - it's inharited so we dont need 'UFUNCTION()' here but it's mean that thay must be marks as 'virtual' then overrided in cpp-file
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
			
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Method for Equiping a weapons
		void Equip(class AMain * Char);

		// Setter for WeaponState
		FORCEINLINE void SetWeaponState(EWeaponState State)
		{
			WeaponState = State;
		}
		// Getter for WeaponState
		FORCEINLINE EWeaponState GetWeaponState()
		{
			return WeaponState;
		}


		// events for CombatCollizion
		UFUNCTION()
		 void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
		UFUNCTION()
		 void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		// Activate weapon collision for playing particles in proper moment of the strike
		UFUNCTION(BlueprintCallable)
		void ActivationCombatCollision();

		// Deactivate weapon collision for stop play particles in proper moment of the strike
		UFUNCTION(BlueprintCallable)
		void DeactivationCombatCollision();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Damage")
		TSubclassOf<UDamageType> DamageTypeClass;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat ")
		AController * WeaponInstigator;

		// Setter for WeaponState
		FORCEINLINE void SetInstigator(AController * Inst)
		{
			WeaponInstigator = Inst;
		}
};
