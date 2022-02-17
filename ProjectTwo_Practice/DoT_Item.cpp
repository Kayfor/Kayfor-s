// Fill out your copyright notice in the Description page of Project Settings.


#include "DoT_Item.h"
#include "Main.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


ADoT_Item::ADoT_Item()
{
	DoT = 10.f;
}


void ADoT_Item::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT(" ADoT_Item::OnOverlap Begins. "));

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Main)
		{
			Main->DoT(DoT);			
			Destroy();
		}
		if(Enemy)
		{
			Enemy->DoT(DoT);			
			Destroy();
		}
	}
}

void ADoT_Item::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT(" ADoT_Item::OnOverlap End. "));
}
