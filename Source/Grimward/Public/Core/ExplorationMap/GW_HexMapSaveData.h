// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GW_HexCoordinate.h"
#include "GW_HexMapSaveData.generated.h"
/*-------------------------------------------------------------------------*/



/**
 * Minimal save data for a single tile
 * Only saves essential progress data, not generated content
 */
USTRUCT(BlueprintType)
struct FTileSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FGW_HexCoordinate Coordinate;

	UPROPERTY(SaveGame)
	bool bIsConquered;

	FTileSaveData()
		: Coordinate(FGW_HexCoordinate())
		, bIsConquered(false)
	{}
};

/**
 * Save game data for the hex map
 * Stores only seed and player progress - map regenerates from seed
 */
UCLASS()
class GRIMWARD_API UGW_HexMapSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UGW_HexMapSaveData();

	/** The seed used to generate this map */
	UPROPERTY(SaveGame)
	int32 MapSeed;

	/** Player's current view range */
	UPROPERTY(SaveGame)
	int32 ViewRange;

	/** All conquered tile coordinates */
	UPROPERTY(SaveGame)
	TArray<FTileSaveData> ConqueredTiles;

	/** Save slot name */
	UPROPERTY(SaveGame)
	FString SaveSlotName;

	/** Last save time */
	UPROPERTY(SaveGame)
	FDateTime LastSaveTime;

	/** Player progression stats */
	UPROPERTY(SaveGame)
	int32 TotalTilesExplored;

	UPROPERTY(SaveGame)
	int32 TotalMegagonsDefeated;
};