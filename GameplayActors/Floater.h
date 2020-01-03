// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS() // Класс наследуемый от 'Actor' уже унаследовал свойство 'Blueprintable' 
class MYTRAININGPROJECT_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = " ActorMeshComponents ") // Создаём новый макрос для переменных/ссылок/указателей
		UStaticMeshComponent * StaticMesh; // и собственно указатель типа UStaticMesh

	// Location used by SetActorLocation() when BeginPlay() is called
	UPROPERTY(EditInstanceOnly,  BlueprintReadWrite, Category = " Floater Variables ")
	FVector InitialLocation ;

	// Location of an Actor when dragged in from the editor
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = " Floater Variables ")
	FVector PlacedLocation;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = " Floater Variables ")
		FVector WorldOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Floater Variables ")
	FVector InitialDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Floater Variables ")
		bool bShouldFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = " Floater Variables ")
		bool bInitializeFloaterLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Floater Variables |  Rotation ")
		bool bInitialRotation;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = " Floater Variables ")
		FVector InitialForce;
	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = " Floater Variables ")
		FVector InitialTorque;

private:

	float RunningTime;

	UPROPERTY(EditAnywhere, Category = " Floater Variables | Wave Parameters ")
	float Amplitude;
	UPROPERTY(EditAnywhere, Category = " Floater Variables | Wave Parameters ")
	float TimeStretch;
	

	UPROPERTY(EditAnywhere, Category = " Floater Variables |  Rotation ")
	float Pitch ;
	UPROPERTY(EditAnywhere, Category = " Floater Variables |  Rotation ")
	float Yaw ;
	UPROPERTY(EditAnywhere, Category = " Floater Variables |  Rotation ")
	float Roll;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
