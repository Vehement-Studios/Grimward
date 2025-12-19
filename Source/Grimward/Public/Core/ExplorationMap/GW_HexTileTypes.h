// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GW_HexTileTypes.generated.h"
/*-------------------------------------------------------------------------*/



/**
 * Base biome types
 */
UENUM(BlueprintType)
enum class EBiomeType : uint8
{
    None            UMETA(DisplayName = "None"),
    Hills           UMETA(DisplayName = "Hills"),
    Forest          UMETA(DisplayName = "Forest"),
    Mountains       UMETA(DisplayName = "Mountains"),
    Desert          UMETA(DisplayName = "Desert"),
    Swamp           UMETA(DisplayName = "Swamp"),
    // Rare biomes
    MysticForest    UMETA(DisplayName = "Mystic Forest"),
    PoisonSwamp     UMETA(DisplayName = "Poison Swamp"),
    DragonBones     UMETA(DisplayName = "Dragon Bones"),
    Lavascape       UMETA(DisplayName = "Lavascape"),
    IceSpikes       UMETA(DisplayName = "Ice Spikes")
};

/**
 * Point of Interest types (structures on tiles)
 */
UENUM(BlueprintType)
enum class EPOIType : uint8
{
    None                UMETA(DisplayName = "None"),
    // Rare POIs
    Monster             UMETA(DisplayName = "Monster"),
    MarauderCamp        UMETA(DisplayName = "Marauder Camp"),
    Ruins               UMETA(DisplayName = "Ruins"),
    Village             UMETA(DisplayName = "Village"),
    Tomb                UMETA(DisplayName = "Tomb"),
    // Epic POIs
    Monolith            UMETA(DisplayName = "Monolith"),
    Oasis               UMETA(DisplayName = "Oasis"),
    WizardTower         UMETA(DisplayName = "Wizard Tower"),
    Crypt               UMETA(DisplayName = "Crypt"),
    Rift                UMETA(DisplayName = "Rift"),
    Forge               UMETA(DisplayName = "Forge"),
    DragonPerch         UMETA(DisplayName = "Dragon Perch"),
    CrystalFields       UMETA(DisplayName = "Crystal Fields"),
    // Megagon POIs
    DragonsDen          UMETA(DisplayName = "Dragon's Den"),
    AncientDragon       UMETA(DisplayName = "Ancient Dragon"),
    MageCastle          UMETA(DisplayName = "Mage Castle"),
    DarkCastle          UMETA(DisplayName = "Dark Castle"),
    Pyrolith            UMETA(DisplayName = "Pyrolith"),
    Cryolith            UMETA(DisplayName = "Cryolith"),
    ForestsOfGrim       UMETA(DisplayName = "Forests of Grim"),
    GabagolGrudge       UMETA(DisplayName = "Gabagol Grudge"),
    CorpseOfNorridia    UMETA(DisplayName = "Corpse of Norridia"),
    MinesOfMortem       UMETA(DisplayName = "Mines of Mortem"),
    TheGreatForge       UMETA(DisplayName = "The Great Forge"),
    DimensionalStronghold UMETA(DisplayName = "Dimensional Stronghold")
};

/**
 * Tile rarity levels
 */
UENUM(BlueprintType)
enum class ETileRarity : uint8
{
    Basic       UMETA(DisplayName = "Basic"),
    Rare        UMETA(DisplayName = "Rare"),
    Epic        UMETA(DisplayName = "Epic"),
    Megagon     UMETA(DisplayName = "Megagon")
};

/**
 * Helper struct to define POI spawn rules
 */
USTRUCT()
struct FPOISpawnRule
{
    GENERATED_BODY()

    UPROPERTY()
    EPOIType POIType;

    UPROPERTY()
    ETileRarity Rarity;

    UPROPERTY()
    TArray<EBiomeType> ValidBiomes;

    UPROPERTY()
    float SpawnWeight; // Higher = more common

    // Special conditions
    UPROPERTY()
    bool bRequiresSpecificBiome;

    UPROPERTY()
    TArray<EPOIType> RequiredNearbyPOIs; // For special spawns like Great Forge

    UPROPERTY()
    int32 RequiredNearbyPOICount;

    FPOISpawnRule()
        : POIType(EPOIType::None)
        , Rarity(ETileRarity::Basic)
        , SpawnWeight(1.0f)
        , bRequiresSpecificBiome(false)
        , RequiredNearbyPOICount(0)
    {}
};