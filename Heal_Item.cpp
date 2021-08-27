// Fill out your copyright notice in the Description page of Project Settings.


#include "Heal_Item.h"
#include "Main.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


//Constructor
AHeal_Item::AHeal_Item()
{
	Heal = 30.f;
}

void AHeal_Item::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("Heal_Item::OnOverlap Begins. "));

	/**  */
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
			Main->IncrementHealth(Heal);			
			Destroy();
		}
	}
	
}

void AHeal_Item::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT(" Heal_Item::OnOverlap End. "));
}
