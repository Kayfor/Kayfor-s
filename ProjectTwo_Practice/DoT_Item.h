// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosive.h"
#include "DoT_Item.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API ADoT_Item : public AExplosive
{
	GENERATED_BODY()

public:
	ADoT_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DoT;



	//- commented because of an error which told that such UFUNCTION macro alredy used in Base class
	//UFUNCTION() 
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	//UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
