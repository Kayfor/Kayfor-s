// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstSaveGameSystem.h"
#include "Main.h"


UFirstSaveGameSystem::UFirstSaveGameSystem() // Constructor
{
	PlayerName = TEXT("Default");
	UserIndex = 0;

	CharacterStats.WeaponName = TEXT("");
	CharacterStats.LevelName = TEXT("");
}

