// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_HexMapGenerator.h"
#include "GW_HexMapTestActor.generated.h"
/*-------------------------------------------------------------------------*/



/**
 * Test actor to demonstrate hex map functionality
 * Place this in your level to test the generation system
 */
UCLASS()
class GRIMWARD_API AGW_HexMapTestActor : public AActor
{
    GENERATED_BODY()

public:
    AGW_HexMapTestActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ===== EDITOR CONTROLS =====

    /** Test seed for map generation */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Controls")
    int32 TestSeed = 12345;

    /** Auto-initialize map on begin play */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Controls")
    bool bAutoInitialize = true;

    /** Show debug visualization */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Controls")
    bool bShowDebugVisualization = true;

    /** How many tiles to show debug for */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Controls")
    int32 DebugRadius = 3;

    /** Hex size for world space conversion */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Controls")
    float HexSize = 150.0f;

    // ===== TEST FUNCTIONS =====

    /** Initialize the map with test seed */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestInitializeMap();

    /** Generate and log several tiles */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestGenerateTiles();

    /** Test conquering a tile and see what unlocks */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestConquerTile(FGW_HexCoordinate Coordinate);

    /** Print map statistics */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void PrintMapStatistics();

    /** Test save and load */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestSaveLoad();

    /** Find and print all rare biomes */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void FindRareBiomes(int32 SearchRadius);

    /** Find and print all megagons */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void FindMegagons(int32 SearchRadius);

    /** Visualize the map in 3D space */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void VisualizeMap();

    UFUNCTION(BlueprintCallable, Category = "Test")
    void GenerateAndUnlockRadius(int32 Radius);

    UFUNCTION(BlueprintCallable, Category = "Test")
    void GenerateAndVisualizeArea(int32 CenterQ, int32 CenterR, int32 Radius);

protected:
    UPROPERTY()
    AGW_HexMapGenerator* MapGenerator;

    void GetBiomeColors(EBiomeType Biome, FColor& OutlineColor, FColor& TextColor) const;
    
    // Helper functions
    FVector HexToWorldPosition(const FGW_HexCoordinate& Coord) const;
    FColor GetBiomeColor(EBiomeType Biome) const;
    FColor GetRarityColor(ETileRarity Rarity) const;
    void DrawHexOutline(const FGW_HexCoordinate& Coord, FColor Color, float Thickness = 5.0f) const;
};