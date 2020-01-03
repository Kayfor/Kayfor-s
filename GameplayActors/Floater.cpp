// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components\StaticMeshComponent.h" // для того чтобы компилятор знал, что есть StaticMesh->


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

	bInitializeFloaterLocations = true;
	bShouldFloat = false;
	bInitialRotation = false;

	InitialForce = FVector(2000000.0f, 0.0f, 0.0f);
	InitialTorque = FVector(2000000.0f, 0.0f, 0.0f);

	RunningTime = 0;

	Amplitude = 1.f;
	TimeStretch = 1.f;
	

}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();
	//'FRand'  set random number between 0 and 1
	float InitialX = FMath::FRandRange(-350.f, 350.f); //'FRandRange' set random number in a range
	float InitialY = FMath::FRandRange(-350.f, 350.f);
	float InitialZ = FMath::FRandRange(0.f, 350.f);

	InitialLocation.X = InitialX;
	InitialLocation.Y = InitialY;
	InitialLocation.Z = InitialZ;	

	PlacedLocation = GetActorLocation();

	if (bInitializeFloaterLocations)
	{
		SetActorLocation(InitialLocation);
	}
	
	/*
	StaticMesh->AddForce(InitialForce);
	StaticMesh->AddTorque(InitialTorque);
	*/

/* This block about World and Local Offsets of the Actor-objects
	FVector LocalOffset = FVector(200.0f, 0.0f, 0.0f);	
	FHitResult HitResult;

    AddActorLocalOffset(LocalOffset, true, &HitResult);
    AddActorWorldOffset(LocalOffset, true, &HitResult);
*/
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bShouldFloat)
		{
		/*
		//FHitResult HitResult; // Заглушка для следующего метода
		//	AddActorLocalOffset(InitialDirection, false, &HitResult );
			// ХитРезалт с амперсандом т.к. сигнатура метода требует 
			// указатель(адрес) в качестве параметра. Так обходим этот момент.

		//	FVector HitLocation = HitResult.Location;
			// Display info from 'HitLocation'
		//	UE_LOG(LogTemp, Warning, TEXT(" Hit Location: X= %f, Y= %f,  Z= %f,  "), HitLocation.X, HitLocation.Y, HitLocation.Z);
	*/	
		
		FVector NewLocation = GetActorLocation(); // Create a new FVector and set to him current position of floater

		//NewLocation.Z = NewLocation.Z + (Amplitude)* FMath::Sin(TimeStretch*RunningTime) ;
		NewLocation.X = NewLocation.X +  (Amplitude)* FMath::Sin(TimeStretch*RunningTime);
		NewLocation.Y = NewLocation.Y + (Amplitude)* FMath::Cos(TimeStretch*RunningTime);

		SetActorLocation(NewLocation);
		RunningTime += DeltaTime;
		}

	/* This code block about  ActorLocal and ActorWorld Rotation*/
	 Pitch = 5.0f;
	 Yaw = 0.0f;
	 Roll = 0.0f;

	FRotator Rotation = FRotator(Pitch, Yaw, Roll);
	if (bInitialRotation)
	{
		AddActorLocalRotation(Rotation); 
	}
	
	
}

