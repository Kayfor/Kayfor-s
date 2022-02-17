// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIController.h"

// CONSTRUCTOR Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if (Actor_1 &&  Actor_2 &&  Actor_3 &&  Actor_4 &&  Actor_5)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
		SpawnArray.Add(Actor_5);
	}	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent(); // how big will box be
	FVector Origin = SpawningBox->GetComponentLocation(); // Hold location point

	// Random values generator
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return Point;
}

// Spawn volume creation.
void ASpawnVolume::SpawnOurActor_Implementation(UClass * ToSpawn, const FVector & Location)
{
	if (ToSpawn)
	{
		UWorld * World = GetWorld(); // Needs in "Engine/World.h"
		FActorSpawnParameters SpawnParams;
		if (World)
		{
			AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(1.f), SpawnParams); // Needs in "Critter.h"
			AEnemy* Enemy = Cast<AEnemy>(Actor);

			if (Enemy)
			{
				Enemy->SpawnDefaultController();

				AAIController* AICont = Cast<AAIController>(Enemy->GetController());
				if (AICont)
				{
					Enemy->AIController = AICont;
				}
			}
		}
	}
}

	TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
	{
			if (SpawnArray.Num() > 0)
			{
				int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);
				return SpawnArray[Selection]; // Return random Actor from array due to index
			}
				else
				{
					return nullptr; // If array is empty - go next )
				}
	}


