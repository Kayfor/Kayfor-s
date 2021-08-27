// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickUp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API APickUp : public AItem
{
	GENERATED_BODY()

public:
	APickUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins")
		int32 CoinCount;

	/** Pay attantion that You MUST use 'class UParticleSystem' here! Not 'class UParticleSystemComponent' !!! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		class UParticleSystem * OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue * OverlapSound;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
		
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
