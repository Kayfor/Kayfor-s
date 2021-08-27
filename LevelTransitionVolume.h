// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolume.generated.h"

UCLASS()
class PROJECTTWO_PRACTICE_API ALevelTransitionVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransitionVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transition")
		class UBoxComponent * LevelTransitionVolume;

	class UBillboardComponent * Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	FName TransitionLevelName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
		class UStaticMeshComponent * PortalStaticMesh;
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
		class USkeletalMeshComponent * PortalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Particles")
		class UParticleSystemComponent * IdlePortalParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		class UParticleSystemComponent * EnterParticles;
*/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition| Sound")
		class USoundCue * PortalSound;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
