// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "Main.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/WorldPSCPool.h"
#include "Sound/SoundCue.h"

//Constructor
APickUp::APickUp()
{
	CoinCount = 1;
}


void APickUp::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT(" PickUp::OnOverlap Begins. "));

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			if (Cast<AMain>(OtherActor))
			{
				if (OverlapParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), false);
				}

				if (OverlapSound)
				{
					UGameplayStatics::PlaySound2D(this, OverlapSound);
				}
			}
			Main->IncrementCoins(CoinCount);
			Main->PickupLocations.Add(GetActorLocation());
			Destroy();
		}
	}
	
}

void APickUp::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT(" PickUp::OnOverlap End. "));
}
