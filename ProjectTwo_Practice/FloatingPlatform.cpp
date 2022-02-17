// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Mesh;
	// now I'm switching to UE and make blueprint for this class 

	// Create base points for platform
	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	InterpSpeed = 1.0f;
	InterpTime = 3.0f;

	bInterping = false;
		
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;

	bInterping = false;

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Let's create Interpolation shifting for our platforms between start and endPoints
	if (bInterping)
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
				ToggleInterping();
				GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
				SwapVectors(StartPoint, EndPoint);
			}
	}
	
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVectors(FVector & VecOne, FVector & VecTwo)
{
	FVector Temp = VecOne; 
	VecOne = VecTwo;
	VecTwo = Temp;
}

