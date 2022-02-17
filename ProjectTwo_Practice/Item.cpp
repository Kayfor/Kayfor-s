// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Main.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollizionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollizionVolume"));
	RootComponent = CollizionVolume;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT(" IdleParticlesComponent"));
	IdleParticleComponent->SetupAttachment(GetRootComponent());

	//OverlapParticles = CreateAbstractDefaultSubobject<UParticleSystem>(TEXT(" OverlapParticles"));
	//OverlapParticles->SetupAttachment(GetRootComponent());

	// default values
	bRotate = false;
	RotationRate = 45;  
}


// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	// Events registartion
	CollizionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollizionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotation realisation pretty simple
	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}

}

/**  I commented this fragment of code because had an issue with 'SpawnEmitterAtLocation' parametres.
		So I realised it with BluePrint
*/
void AItem::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT(" Super::OnOverlapBegins() "));	
	//Functionality was transfered directly to the Explosive.cpp
}

void AItem::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT(" Super::OnOverlapEnd() "));
}

