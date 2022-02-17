// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Main.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/WorldPSCPool.h"
#include "Sound/SoundCue.h"

//Constructor
AExplosive::AExplosive()
{
	DamageAmount = 15.f;
}


void AExplosive::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT(" Explosive::OnOverlap Begins. "));
	
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Main || Enemy)
		{
			// Let's spawn the particles
			if (Cast<AMain>(OtherActor) || Cast<AEnemy>(OtherActor))
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
			//Main->DecrementHealth(DamageAmount);
			UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, nullptr, this, DamageTypeClass);
			Destroy();
		}
	}
	
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT(" Explosive::OnOverlap End. "));
}
