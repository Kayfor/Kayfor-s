// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Heal_Item.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API AHeal_Item : public AItem
{
	GENERATED_BODY()
	
public:
	AHeal_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
		float Heal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		class UParticleSystem * OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue * OverlapSound;

	//- commented because of an error which told that such UFUNCTION macro alredy used in Base class
	//UFUNCTION() 
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	//UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
