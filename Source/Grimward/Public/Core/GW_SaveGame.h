// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GW_GameData.h"
#include "GW_SaveGame.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_SaveGame.h
UCLASS()
class GRIMWARD_API UGW_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UGW_SaveGame();

	UPROPERTY(VisibleAnywhere, Category = "Grimward|Save")
	FGW_PlayerProfile PlayerProfile;

	UPROPERTY(VisibleAnywhere, Category = "Grimward|Save")
	FDateTime SaveTime;
};
#pragma endregion 
/*-------------------------------------------------------------------------*/