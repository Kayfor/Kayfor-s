// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class PROJECTTWO_PRACTICE_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	/** Mesh for the Platform */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	class UStaticMeshComponent * Mesh;

	//Vector for 'StartPont'
	UPROPERTY(EditAnywhere)
	FVector StartPoint;

	//Vector for 'EndPont'
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "True"))
		FVector EndPoint;

	// Platform Interpolation speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpSpeed;

	// Platform Interpolation transition time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		float InterpTime;

	// Timer for cycling interpolation between points
	FTimerHandle InterpTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bInterping;

	float Distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();

	void SwapVectors(FVector & VecOne, FVector & VecTwo);

};
