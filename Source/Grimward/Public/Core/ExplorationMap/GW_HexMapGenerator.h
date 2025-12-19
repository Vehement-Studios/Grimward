// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GW_HexCoordinate.h"
#include "GW_HexTileData.h"
#include "GW_HexTileTypes.h"
#include "GW_SeededRandom.h"
#include "GameFramework/Actor.h"
#include "GW_HexMapGenerator.generated.h"
/*-------------------------------------------------------------------------*/


/**
 * Configuration for map generation
 */
USTRUCT(BlueprintType)
struct FMapGenerationConfig
{
    GENERATED_BODY()

    // Biome generation weights (higher = more common)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Biomes")
    float HillsWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Biomes")
    float ForestWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Biomes")
    float MountainsWeight = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Biomes")
    float DesertWeight = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Biomes")
    float SwampWeight = 0.6f;

    // Rare biome chances (0.0 to 1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Rare Biomes")
    float MysticForestChance = 0.15f; // 15% of forests

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Rare Biomes")
    float PoisonSwampChance = 0.15f; // 15% of swamps

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Rare Biomes")
    float DragonBonesChance = 0.05f; // 5% of deserts

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Rare Biomes")
    float LavascapeChance = 0.2f; // 20% of mountains (mutually exclusive with ice)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Rare Biomes")
    float IceSpikesChance = 0.2f; // 20% of mountains (mutually exclusive with lava)

    // POI spawn chances by rarity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|POI")
    float RarePOIChance = 0.3f; // 30% of basic tiles get rare POI

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|POI")
    float EpicPOIChance = 0.1f; // 10% of rare tiles get epic POI

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|POI")
    float MegagonChance = 0.02f; // 2% chance per suitable area

    // Biome clustering (how much biomes cluster together)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Clustering")
    float BiomeClusteringStrength = 0.6f; // 0 = random, 1 = heavy clustering

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Clustering")
    int32 BiomeClusterRadius = 2; // How far to check for same biome neighbors
};

/**
 * Main hex map generator and data manager
 */
UCLASS()
class GRIMWARD_API AGW_HexMapGenerator : public AActor
{
    GENERATED_BODY()

public:
    AGW_HexMapGenerator();

protected:
    virtual void BeginPlay() override;

public:
    // ===== PUBLIC API =====

    /** Initialize the map with a seed and starting position */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void InitializeMap(int32 InSeed, FGW_HexCoordinate StartingCoordinate);

    /** Get tile data for a specific coordinate (generates if needed) */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    FGW_HexTileData GetTileData(const FGW_HexCoordinate& Coordinate);

    /** Check if a tile exists (has been generated) */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    bool DoesTileExist(const FGW_HexCoordinate& Coordinate) const;

    /** Conquer a tile (mark as complete) */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void ConquerTile(const FGW_HexCoordinate& Coordinate, int32 ViewRange = 1);

    /** Get all conquered tiles */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    TArray<FGW_HexCoordinate> GetConqueredTiles() const;

    /** Get all unlocked tiles */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    TArray<FGW_HexCoordinate> GetUnlockedTiles() const;

    /** Get all visible tiles (within view range of conquered tiles) */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    TArray<FGW_HexCoordinate> GetVisibleTiles() const;

    /** Update visibility for all tiles based on conquered tiles and view range */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void UpdateVisibility(int32 ViewRange = 1);

    /** Get the current seed */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    int32 GetMapSeed() const { return MapSeed; }

    /** Get number of conquered tiles */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    int32 GetConqueredTileCount() const;

    // ===== CONFIGURATION =====

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
    FMapGenerationConfig GenerationConfig;

    /** Save the current map state */
    UFUNCTION(BlueprintCallable, Category = "Hex Map|Save")
    bool SaveMapState(const FString& SlotName = TEXT("HexMapSave"));

    /** Load a saved map state */
    UFUNCTION(BlueprintCallable, Category = "Hex Map|Save")
    bool LoadMapState(const FString& SlotName = TEXT("HexMapSave"));

    /** Check if a save exists */
    UFUNCTION(BlueprintCallable, Category = "Hex Map|Save")
    static bool DoesSaveExist(const FString& SlotName = TEXT("HexMapSave"));

    /** Delete a save */
    UFUNCTION(BlueprintCallable, Category = "Hex Map|Save")
    static bool DeleteSave(const FString& SlotName = TEXT("HexMapSave"));

    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void ForceUnlockTile(const FGW_HexCoordinate& Coordinate);

    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void ForceUnlockArea(const FGW_HexCoordinate& Center, int32 Radius);
    
protected:
    // ===== INTERNAL DATA =====

    /** Map of all generated tiles */
    UPROPERTY()
    TMap<FGW_HexCoordinate, FGW_HexTileData> GeneratedTiles;

    /** Current map seed */
    UPROPERTY()
    int32 MapSeed;

    /** Random generator */
    FGW_SeededRandom RandomGenerator;

    /** Coordinates that have been marked for megagon generation */
    TSet<FGW_HexCoordinate> MegagonCenters;

    /** POI spawn rules (initialized in constructor) */
    TArray<FPOISpawnRule> POISpawnRules;

    // ===== INTERNAL GENERATION METHODS =====

    /** Generate a single tile at the given coordinate */
    FGW_HexTileData GenerateTile(const FGW_HexCoordinate& Coordinate);

    /** Generate base biome for a coordinate */
    EBiomeType GenerateBiome(const FGW_HexCoordinate& Coordinate);

    /** Try to upgrade to rare biome */
    EBiomeType TryUpgradeToRareBiome(EBiomeType BaseBiome, const FGW_HexCoordinate& Coordinate);

    /** Generate POI for a tile */
    EPOIType GeneratePOI(const FGW_HexCoordinate& Coordinate, EBiomeType Biome);

    /** Determine tile rarity based on biome and POI */
    ETileRarity DetermineTileRarity(EBiomeType Biome, EPOIType POI) const;

    /** Check if a megagon can spawn at this location */
    bool CanSpawnMegagon(const FGW_HexCoordinate& Coordinate, EBiomeType Biome);

    /** Try to spawn a megagon at this location */
    EPOIType TrySpawnMegagon(const FGW_HexCoordinate& Coordinate, EBiomeType Biome);

    /** Get valid POIs for a given biome and rarity */
    TArray<EPOIType> GetValidPOIsForBiome(EBiomeType Biome, ETileRarity Rarity) const;

    /** Check if POI can spawn on this biome */
    bool CanPOISpawnOnBiome(EPOIType POI, EBiomeType Biome) const;

    /** Get biome weight for clustering */
    float GetBiomeClusteringWeight(const FGW_HexCoordinate& Coordinate, EBiomeType TestBiome);

    /** Count nearby POIs of specific type */
    int32 CountNearbyPOIs(const FGW_HexCoordinate& Coordinate, EPOIType POIType, int32 Range) const;

    /** Initialize POI spawn rules */
    void InitializePOISpawnRules();

    /** Unlock adjacent tiles when a tile is conquered */
    void UnlockAdjacentTiles(const FGW_HexCoordinate& Coordinate);

    /** Generate visual variant for a tile */
    int32 GenerateVisualVariant(const FGW_HexCoordinate& Coordinate, EBiomeType Biome);

    UPROPERTY()
    int32 CurrentViewRange;
};