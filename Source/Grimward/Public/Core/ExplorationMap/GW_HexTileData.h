// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GW_HexCoordinate.h"
#include "GW_HexTileTypes.h"
#include "GW_HexTileData.generated.h"
/*-------------------------------------------------------------------------*/



/**
 * Complete data for a single hex tile
 */
USTRUCT(BlueprintType)
struct FGW_HexTileData
{
    GENERATED_BODY()

    // Position
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
    FGW_HexCoordinate Coordinate;

    // Terrain/Biome
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
    EBiomeType BiomeType;

    // Structure/POI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
    EPOIType POIType;

    // Rarity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
    ETileRarity Rarity;

    // Unlock and completion status
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Progress")
    bool bIsUnlocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Progress")
    bool bIsConquered;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Progress")
    bool bIsVisible; // Within view range

    // Megagon data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Megagon")
    bool bIsMegagonCenter; // Is this the center of a megagon tile?

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Megagon")
    bool bIsPartOfMegagon; // Is this part of a megagon cluster?

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Megagon")
    FGW_HexCoordinate MegagonCenter; // If part of megagon, where is the center?

    // Level association
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Level")
    FString LevelName; // Scene/level to load

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Level")
    int32 DifficultyLevel;

    // Visual variation (for multiple sprites of same biome)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual")
    int32 VisualVariant; // 0-N for different sprites

    FGW_HexTileData()
        : Coordinate(FGW_HexCoordinate())
        , BiomeType(EBiomeType::None)
        , POIType(EPOIType::None)
        , Rarity(ETileRarity::Basic)
        , bIsUnlocked(false)
        , bIsConquered(false)
        , bIsVisible(false)
        , bIsMegagonCenter(false)
        , bIsPartOfMegagon(false)
        , MegagonCenter(FGW_HexCoordinate())
        , DifficultyLevel(1)
        , VisualVariant(0)
    {}

    // Helper to check if tile can be interacted with
    bool CanEnter() const
    {
        return bIsUnlocked && !bIsConquered;
    }

    // Get display name for this tile
    FString GetDisplayName() const
    {
        if (POIType != EPOIType::None)
        {
            return UEnum::GetDisplayValueAsText(POIType).ToString();
        }
        return UEnum::GetDisplayValueAsText(BiomeType).ToString();
    }
};