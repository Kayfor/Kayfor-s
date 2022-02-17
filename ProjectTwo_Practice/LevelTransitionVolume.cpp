// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Main.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // because we don't need it here

	LevelTransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("LevelTransitionVolume"));
	RootComponent = LevelTransitionVolume; // Add it in UE submenu

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent()); // Add it in UE submenu

	/*
	IdlePortalParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdlePortalParticles"));
	IdlePortalParticles->SetupAttachment(GetRootComponent());

	EnterParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnterParticles"));
	EnterParticles->SetupAttachment(GetRootComponent());
*/
	PortalStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalStaticlMesh"));
	PortalStaticMesh->SetupAttachment(GetRootComponent());
	/*
	PortalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PortalSkeletalMesh"));
	PortalMesh->SetupAttachment(GetRootComponent());
*/
	//PortalSound = CreateDefaultSubobject<USoundCue>(TEXT (" PortalSound "));
	//PortalSound->SetupAttachment(GetRootComponent());  //UGameplayStatics::PlaySound2D(this, OverlapSound);

	TransitionLevelName = "SunTemple";
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();
	
	LevelTransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{		
		AMain * Main	= Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->Savegame();  // Save parametres before teleport to new location
			Main->SwitchLevel(TransitionLevelName);		
		}
			
	}
	/*
	if (EnterParticles)
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EnterParticles, GetActorLocation(), FRotator(0.f), false);
	}
   */
}

void ALevelTransitionVolume::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
  
}

