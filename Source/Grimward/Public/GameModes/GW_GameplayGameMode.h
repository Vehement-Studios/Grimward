// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Core/ExplorationMap/GW_MapGenerator.h"
#include "GameFramework/GameModeBase.h"
#include "UI/Menus/GW_MapGeneratorWidget.h"
#include "GW_GameplayGameMode.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Menu Game Mode                                                         */
/*-------------------------------------------------------------------------*/
#pragma region GW_GameplayGameMode.h
UCLASS()
class GRIMWARD_API AGW_GameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGW_GameplayGameMode();

	virtual void BeginPlay() override;
	
	UFUNCTION(Exec)
	void GenerateDebugMap(int32 Seed = 12345);
	
protected:
	// Background music for gameplay
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|Audio")
	USoundBase* GameplayMusic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|Display")
	UGW_MapGeneratorWidget* MapGeneratorWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|UI")
	TSubclassOf<class UGW_MapGeneratorWidget> MapGeneratorWidgetClass;

	
private:
	void SetupGameplayInput();
	
	UPROPERTY()
	AGW_MapGenerator* MapGenerator; // Temp for testing
	
	UPROPERTY()
	UTexture2D* MapDebugTexture; // Temp for testing
};
#pragma endregion
/*-------------------------------------------------------------------------*/