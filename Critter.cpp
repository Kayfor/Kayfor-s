// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
#include "Components/StaticMeshComponent.h" //gives an able to use 'StaticMeshComponent' here)
#include "Camera/CameraComponent.h"				 //gives an able to use 'CameraComponent' here)
#include "Components/InputComponent.h"			 //gives an able to use 'InputComponent' here)

// Sets default values
ACritter::ACritter() // Constructor
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootCompponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT(" MeshComponent "));
	MeshComponent->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT(" Camera "));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(-300.f, 0.f, 300.f));
	Camera->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	AutoPossessPlayer = EAutoReceiveInput::Player1;

	CurrentVelocity = FVector(0.f);
	MaxSpeed = 100.f;
}

// Called when the game starts or when spawned
void ACritter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	SetActorLocation(NewLocation);

}

// Called to bind functionality to input
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward/Backward"), this , &ACritter::MoveForwardBackward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight/Left"), this, &ACritter::MoveRightLeft);
	PlayerInputComponent->BindAxis(TEXT("MoveJump"), this, &ACritter::MoveJump);
}


// Методы-заглушки для корректного вызова 'BindAxis' 
void ACritter::MoveForwardBackward(float value)
{
	CurrentVelocity.X = FMath::Clamp(value, -1.f, 1.f) * MaxSpeed; 
	// 'Clamp' сводит любые значения к указанному диапазону 
}

void ACritter::MoveRightLeft(float value)
{
	CurrentVelocity.Y = FMath::Clamp(value, -1.f, 1.f) * MaxSpeed;
}

void ACritter::MoveJump(float value)
{
	CurrentVelocity.Z = FMath::Clamp(value, 0.f, 1.f) * MaxSpeed;
}

