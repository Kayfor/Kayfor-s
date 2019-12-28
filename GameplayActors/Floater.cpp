// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"

// Sets default values
AFloater::AFloater() // Constructor
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// создаём параметр типа  UStaticMesh 
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaicMesh"));

	// Set default values which initialize this variables
	InitialLocation = FVector(0.0f, 0.0f, 0.0f);
	PlacedLocation =  FVector(0.0f, 0.0f, 0.0f);
	WorldOrigin = FVector(0.0f, 0.0f, 0.0f);

	InitialDirection = FVector(0.0f, 0.0f, 0.0f);

	bInitializeFloaterLocations = false;
	bShouldFloat = false;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	PlacedLocation = GetActorLocation();

	if (bInitializeFloaterLocations)
	{
		SetActorLocation(InitialLocation);
	}
	
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bShouldFloat)
		{
		FHitResult HitResult; // Заглушка для следующего метода
			AddActorLocalOffset(InitialDirection, false, &HitResult );
			// ХитРезалт с амперсандом т.к. сигнатура метода требует 
			// указатель(адрес) в качестве параметра. Так обходим этот момент.
		}

}

