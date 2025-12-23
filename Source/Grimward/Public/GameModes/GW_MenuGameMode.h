// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GW_MenuGameMode.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Menu Game Mode                                                         */
/*-------------------------------------------------------------------------*/
#pragma region GW_MenuGameMode.h
UCLASS()
class GRIMWARD_API AGW_MenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGW_MenuGameMode();

	virtual void BeginPlay() override;

protected:
	// Reference to the main menu widget class (set in Blueprint)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|UI")
	TSubclassOf<class UGW_MainMenuWidget> MainMenuWidgetClass;

	// Instance of the created widget
	UPROPERTY()
	UUserWidget* MainMenuWidget;

	// Background music for menu
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|Audio")
	USoundBase* MenuMusic;

	
private:
	void CreateAndShowMainMenu();
};
#pragma endregion 
/*-------------------------------------------------------------------------*/