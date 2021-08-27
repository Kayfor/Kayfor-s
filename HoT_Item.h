// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HoT_Item.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API AHoT_Item : public AItem
{
	GENERATED_BODY()
public:
	AHoT_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
		float HoT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		class UParticleSystem * OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue * OverlapSound;

	void Tick(float DeltaTime);

	/*
	UPROPERTY(EditAnywhere, Category = "Disable")
		bool HideInGame;
	UFUNCTION(BlueprintCallable, Category = "Disable")
		void DisableActor(bool toHide);*/
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
		
};
