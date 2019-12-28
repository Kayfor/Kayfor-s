// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject.h"

UMyObject::UMyObject()
{
	MyFloat = 0;
}

void UMyObject::MyFunction()
{
	UE_LOG(LogTemp, Warning, TEXT(" This is our warning text! \n"));
}

void UMyObject::MySecondFunction()
{
	UE_LOG(LogTemp, Warning, TEXT(" Test literal inside UE4 log file! \n"));
}
