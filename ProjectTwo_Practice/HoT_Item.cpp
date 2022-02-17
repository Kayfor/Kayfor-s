// Fill out your copyright notice in the Description page of Project Settings.


#include "HoT_Item.h"
#include "Main.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AHoT_Item::AHoT_Item()
{
	HoT = 5.f;	
}

// Called every frame
void AHoT_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
void AHoT_Item::DisableActor(bool toHide)
{
	// Hides visible components
	SetActorHiddenInGame(toHide);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	//SetActorTickEnabled(false);
}*/

void AHoT_Item::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("Heal_Item::OnOverlap Begins. "));
	/** 
	HideInGame = true;

	if (HideInGame)
	{
		DisableActor(HideInGame);
	}	
	 */
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			if (OverlapParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), false);
			}

			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}
						Main->RestoreHealth(HoT);			
		}		
	}      
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("HoT_Item has been destroyed! "));
}

void AHoT_Item::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("HoT_Item::OnOverlap Ends. "));
}


