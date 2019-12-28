// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater_2.generated.h"

UCLASS()
class MYTRAININGPROJECT_API AFloater_2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater_2();

	UPROPERTY(VisibleAnywhere, Category = " ActorMeshComponents ")
		UStaticMeshComponent * StaticMesh_2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
