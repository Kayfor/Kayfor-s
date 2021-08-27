// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGameSystem.generated.h"

// Create a new struct
USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float Health;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float Stamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		int32 Coins;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString WeaponName;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FString LevelName;

};



/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API UFirstSaveGameSystem : public USaveGame
{
	GENERATED_BODY()

public:
		UFirstSaveGameSystem();

		// Create an Exemplar of our struct here to have an acces to it's fields
		UPROPERTY(VisibleAnywhere, Category = Basic)
		FCharacterStats CharacterStats;

		UPROPERTY(VisibleAnywhere, Category = Basic)
		FString PlayerName;

		UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;	

};
