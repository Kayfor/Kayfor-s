// Fill out your copyright notice in the Description page of Project Settings.

#pragma once     // Class which contain projectile behavior after overlapping the target

#include "CoreMinimal.h"
#include "Item.h"
#include "TimerManager.h"
#include "Projectile_Item.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API AProjectile_Item : public AItem
{
	GENERATED_BODY()
	
protected:
	//virtual void BeginPlay() override;

public:
	// Made constructor
	AProjectile_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamageAmount;

	/** Pay attantion that You MUST use 'class UParticleSystem' here! Not 'class UParticleSystemComponent' !!! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		class UParticleSystem * OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue * OverlapSound;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;


	// timer for projectile deletion after certain time(~ 3 sec)
	FTimerHandle ProjectileLifeTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
		float ProjectileLifeTime;

	//void Projectile_Item_Destroy();
};
