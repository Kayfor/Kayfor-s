// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Item.h"
#include "Main.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/WorldPSCPool.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

//Constructor
AProjectile_Item::AProjectile_Item()
{
	DamageAmount = 30.f;
	ProjectileLifeTime = 3.f;
}
	


// to do - finish timer for destroying projectiles
// GetWorldTimerManager().SetTimer(ProjectileLifeTimer, this, &Projectile_Item::Projectile_Item_Destroy, ProjectileLifeTime, true);

/*
 void AProjectile_Item::Projectile_Item_Destroy()
{	

	if (ProjectileLifeTimer.IsValid() )  // stop timer when 'capacity' reach specific value 
	{
		GetWorld()->GetTimerManager().ClearTimer(ProjectileLifeTimer);
		UE_LOG(LogTemp, Warning, TEXT("ProjectileLifeTimer is over! "));
	}
	Destroy();
}*/



// Projectile overlaping behaviour
void AProjectile_Item::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT(" Fireball overlap something! "));

	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			// Let's spawn the particles
			if ( Cast<AEnemy>(OtherActor))
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

void AProjectile_Item::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
  // placeholder
}


