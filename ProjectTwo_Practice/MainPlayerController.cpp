// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Main.h"




void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAssert)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAssert);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar)
	{	
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
	
			if (EnemyHealthBar)
			{
				EnemyHealthBar->AddToViewport();
				EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
			}
			FVector2D Vec(0.f, 0.f);
			EnemyHealthBar->SetAlignmentInViewport(Vec);
	}

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);

		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}		
	}
}

void AMainPlayerController::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 70.f;
		PositionInViewport.X -= 70.f;

		FVector2D SizeInViewport = FVector2D(300.f, 25.f);
		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
		{
			bPauseMenuVisible = true;
			PauseMenu->SetVisibility(ESlateVisibility::Visible);

			// Switch InputMode to able access of Pause Menu
			FInputModeGameAndUI  InputModeGameAndUI;
			SetInputMode(InputModeGameAndUI);
			bShowMouseCursor = true;

						
		}
	}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		GameModeOnly();
		bPauseMenuVisible = false;
		//PauseMenu->SetVisibility(ESlateVisibility::Hidden); - we done with this in bluePrint

		bShowMouseCursor = false;
		
	}
}

void AMainPlayerController::TogglePauseMwnu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::GameModeOnly()
{
	// Switch InputMode back after exit from Pause Menu
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}