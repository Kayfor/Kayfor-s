// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API AExplosive : public AItem
{
	GENERATED_BODY()

public:
	AExplosive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount;

	/** Pay attantion that You MUST use 'class UParticleSystem' here! Not 'class UParticleSystemComponent' !!! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		class UParticleSystem * OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue * OverlapSound;

	//- commented because of an error which told that such UFUNCTION macro alredy used in Base class
	//UFUNCTION() 
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	//UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;
};
