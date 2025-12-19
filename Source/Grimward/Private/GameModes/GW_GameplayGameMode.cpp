// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Public/GameModes/GW_GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_GameplayGameMode.cpp
AGW_GameplayGameMode::AGW_GameplayGameMode()
{
	UE_LOG(LogTemp, Log, TEXT("[Grimward] Gameplay Game Mode Created"));
	
}

void AGW_GameplayGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("[Grimward] Gameplay Game Mode BeginPlay"));

	// Play gameplay music
	if (GameplayMusic)
	{
		// TODO: Add dynamic gameplay Music System here
		
		// UGameplayStatics::PlaySound2D(this, GameplayMusic, 0.5f, 1.0f, 0.0f);
	}

	// Set proper input mode for gameplay
	SetupGameplayInput();
}

void AGW_GameplayGameMode::SetupGameplayInput()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		// Set input mode to game only (allows character movement)
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
        
		// Hide mouse cursor during gameplay
		PC->bShowMouseCursor = false;
        
		UE_LOG(LogTemp, Log, TEXT("[Grimward] Input mode set to Game Only, cursor hidden"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Grimward] Could not get Player Controller in SetupGameplayInput"));
	}
}
#pragma endregion
/*-------------------------------------------------------------------------*/