// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater_2.h"

// Sets default values
AFloater_2::AFloater_2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT(" CustomStaticMesh_2 "));

}

// Called when the game starts or when spawned
void AFloater_2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloater_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

