// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

protected:
	// Background music for gameplay
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|Audio")
	USoundBase* GameplayMusic;

private:
	void SetupGameplayInput();
};
#pragma endregion
/*-------------------------------------------------------------------------*/