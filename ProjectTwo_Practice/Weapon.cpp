// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Main.h"
#include "Enemy.h"

// constructor declaration
AWeapon::AWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT(" SkeletalMesh "));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	 bWeaponParticle = false;

	 WeaponState = EWeaponState::EWS_Pickup;

	 CombatCollizion = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollizion"));
	 CombatCollizion->SetupAttachment(GetRootComponent());

	 Damage = 20.f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();  // call base class realisation

	CombatCollizion->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollizion->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);


	// Setup overlapping particles for proper collisions
	CombatCollizion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollizion->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollizion->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollizion->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AItem::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (WeaponState == EWeaponState::EWS_Pickup &&  OtherActor) // Equip weapon
	{
		AMain * Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(this);
		}
		//Main->SetActiveOverlappingItem(nullptr);
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AItem::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor) // Equip weapon
	{
		AMain * Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(nullptr);
			CombatCollizion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AWeapon::Equip(AMain * Char)
{
	if (Char)  // if Char is valid
	{ 
		SetInstigator(Char->GetController());
		
		// Collisions of weapon sceletal mesh should not effect collisions of camera or pawn
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		// stop simulating phisics for weapon when it equiping
		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket * RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");

		if (RightHandSocket) // if socket is valid -> add skeletal mesh to it
		{
			RightHandSocket->AttachActor(this, Char->GetMesh());
			bRotate = false;  // stop rotation in the socket


			Char->SetEquippedWeapon(this); // Take overlapped wepon to the soket
			Char->SetActiveOverlappingItem(nullptr); // deactivate weapon colision

			WeaponState = EWeaponState::EWS_Equipped;
		}
		// play 2D sound one time when it's equiping
		if (OnEquipSound)
			UGameplayStatics::PlaySound2D(this, OnEquipSound);

		if (!bWeaponParticle) // 0n/Off particles effect
		{
			IdleParticleComponent->Deactivate();
		}
	}
}

// declaration of events for CombatCollizion
void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//AWeapon::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT(" CombatOnOverlap Begin. "));

	if (OtherActor)
	{
		AEnemy * Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy) // if cast was seccessful
		{
			if (Enemy->HitParticles)
			{
				const USkeletalMeshSocket * WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
				if (WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, GetActorLocation(), FRotator(0.f), true);
				}				
			}
			if (Enemy->HitSound && ECollisionEnabled::QueryOnly)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
			}

			// Make damage to enemy
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
			}
		}
	}

}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//AWeapon::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT(" CombatOnOverlap End. "));
	AMain * Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		Main->SetActiveOverlappingItem(nullptr);
		CombatCollizion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}



void AWeapon::ActivationCombatCollision()
{
	CombatCollizion->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	
}

void AWeapon::DeactivationCombatCollision()
{
	CombatCollizion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
