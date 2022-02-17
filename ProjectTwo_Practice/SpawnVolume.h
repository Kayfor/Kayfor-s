// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class PROJECTTWO_PRACTICE_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// form to visualize object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	class UBoxComponent * SpawningBox;
	
	// PawnToSpawn let us an able to choose any SUBclass of 'Aclass' for spawning
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")		
		TSubclassOf< AActor> Actor_1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf< AActor> Actor_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf< AActor> Actor_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf< AActor> Actor_4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf< AActor> Actor_5;

	// We should create an array for this spawning actors:

	TArray<TSubclassOf< AActor>> SpawnArray;

	//TSubclassOf<class  ACritter> PawnToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetSpawnPoint();

	UFUNCTION(BlueprintPure, Category = "Spawning")
		TSubclassOf<AActor> GetSpawnActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawning")
		void SpawnOurActor(UClass* ToSpawn, const FVector& Location);
	
};
