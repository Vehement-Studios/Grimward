// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Public/GameModes/GW_MenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Game Data                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_MenuGameMode.h
AGW_MenuGameMode::AGW_MenuGameMode()
{
	// No pawn needed in menu
	DefaultPawnClass = nullptr;
    
	// No HUD needed (UI handles everything)
	HUDClass = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[Grimward] Menu Game Mode Created"));
}

void AGW_MenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("[Grimward] Menu Game Mode BeginPlay"));

	// Play menu music
	if (MenuMusic)
	{
		UGameplayStatics::PlaySound2D(this, MenuMusic, 0.5f, 1.0f, 0.0f);
	}

	// Create and show main menu
	CreateAndShowMainMenu();
}

void AGW_MenuGameMode::CreateAndShowMainMenu()
{
	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[Grimward] MainMenuWidgetClass is not set! Please set it in BP_GW_MenuGameMode"));
		return;
	}

	// Create the widget
	MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
    
	if (!MainMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("[Grimward] Failed to create Main Menu Widget"));
		return;
	}

	// Add to viewport
	MainMenuWidget->AddToViewport(0);
	UE_LOG(LogTemp, Log, TEXT("[Grimward] Main Menu Widget added to viewport"));

	// Set input mode to UI only
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
        
		UE_LOG(LogTemp, Log, TEXT("[Grimward] Input mode set to UI only with mouse cursor visible"));
	}
}
#pragma endregion
/*-------------------------------------------------------------------------*/