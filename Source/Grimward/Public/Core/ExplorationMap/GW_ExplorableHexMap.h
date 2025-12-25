// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/ExplorationMap/GW_TileTypes.h"
#include "GW_ExplorableHexMap.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/
class UCanvasPanel;
class UGW_HexTile;
class AGW_MapGenerator;
struct FGW_HexTileData;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Hex Map Configuration                                                  */
/*-------------------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FGW_HexMapConfig
{
    GENERATED_BODY()
    
    /** Hex width in pixels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HexWidth = 150.0f;
    
    /** Hex height in pixels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HexHeight = 130.0f;
    
    /** How many tiles to render beyond viewport edges */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TileRenderBuffer = 2;
    
    /** Zoom range */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D ZoomRange = FVector2D(0.5f, 2.0f);
    
    FGW_HexMapConfig() {}
};

/*-------------------------------------------------------------------------*/
/*  Explorable Hexagon Map                                                 */
/*-------------------------------------------------------------------------*/
#pragma region GW_ExplorableHexMap.h
UCLASS()
class GRIMWARD_API UGW_ExplorableHexMap : public UUserWidget
{
    GENERATED_BODY()

protected:
    // ========================================
    // Widget Components
    // ========================================
    
    /** Main canvas that holds all hex tiles */
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* MapCanvas;
    
    // ========================================
    // Configuration
    // ========================================
    
    /** Hex tile widget class to spawn */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex Map")
    TSubclassOf<UGW_HexTile> HexTileClass;
    
    /** Map configuration settings */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex Map")
    FGW_HexMapConfig MapConfig;
    
    /** Reference to map generator */
    UPROPERTY(BlueprintReadWrite, Category = "Hex Map")
    AGW_MapGenerator* MapGenerator;
    
    /** Current map seed being displayed */
    UPROPERTY(BlueprintReadOnly, Category = "Hex Map")
    int32 CurrentMapSeed;
    
    // ========================================
    // Viewport & Camera State
    // ========================================
    
    /** Current viewport offset (for panning) */
    UPROPERTY(BlueprintReadOnly, Category = "Hex Map")
    FVector2D ViewportOffset;
    
    /** Current zoom level */
    UPROPERTY(BlueprintReadOnly, Category = "Hex Map")
    float CurrentZoom;
    
    /** Last center grid position (for culling) */
    UPROPERTY(BlueprintReadOnly, Category = "Hex Map")
    FIntPoint LastCenterGridPos;
    
    /** Currently active/selected tile */
    UPROPERTY(BlueprintReadOnly, Category = "Hex Map")
    UGW_HexTile* ActiveTile;
    
    // ========================================
    // Tile Management
    // ========================================
    
    /** All currently spawned tiles (for pooling/culling) */
    UPROPERTY()
    TMap<FIntPoint, UGW_HexTile*> SpawnedTiles;
    
    /** Tile pool for reuse */
    UPROPERTY()
    TArray<UGW_HexTile*> TilePool;
    
    // ========================================
    // Input State
    // ========================================
    
    /** Is user currently dragging the map */
    bool bIsDragging;
    
    /** Last mouse position for drag calculations */
    FVector2D LastDragPosition;

public:
    // ========================================
    // Initialization
    // ========================================
    
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
    /** Initialize map with a specific seed and starting position */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void InitializeMap(AGW_MapGenerator* InMapGenerator, int32 MapSeed, FIntPoint StartingGridPos);

    /** Initialize map with a specific seed at origin */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void InitializeMapAtOrigin(AGW_MapGenerator* InMapGenerator, int32 MapSeed);
    
    // ========================================
    // Core Tile Management
    // ========================================
    
    /** Build a hex tile widget with data from generator */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    UGW_HexTile* BuildHexTile(FIntPoint GridPos, int32 MapSeed);
    
    /** Add tile to the map canvas at correct position */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void AddTileToScreen(UGW_HexTile* Tile);
    
    /** Remove tile from screen (for culling) */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void RemoveTileFromScreen(UGW_HexTile* Tile);
    
    /** Update visible tiles based on current viewport */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void UpdateVisibleTiles();
    
    // ========================================
    // Coordinate Conversion
    // ========================================
    
    /** Convert axial hex coordinates to pixel position */
    UFUNCTION(BlueprintPure, Category = "Hex Map")
    FVector2D GridToPixel(FIntPoint GridPos) const;
    
    /** Convert pixel position to axial hex coordinates */
    UFUNCTION(BlueprintPure, Category = "Hex Map")
    FIntPoint PixelToGrid(FVector2D PixelPos) const;
    
    /** Get screen position accounting for viewport offset and zoom */
    UFUNCTION(BlueprintPure, Category = "Hex Map")
    FVector2D GetScreenPosition(FVector2D WorldPixelPos) const;
    
    // ========================================
    // Camera Control
    // ========================================
    
    /** Pan the viewport by delta */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void PanViewport(FVector2D Delta);
    
    /** Set zoom level (clamped to config range) */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void SetZoom(float NewZoom);
    
    /** Center viewport on specific grid position */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void CenterOnGridPosition(FIntPoint GridPos);
    
    // ========================================
    // Input Handling
    // ========================================
    
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    
    // ========================================
    // Tile Selection
    // ========================================
    
    /** Set the active tile and trigger popup animation */
    UFUNCTION(BlueprintCallable, Category = "Hex Map")
    void SetActiveTile(UGW_HexTile* Tile);
    
    /** Get currently active tile */
    UFUNCTION(BlueprintPure, Category = "Hex Map")
    UGW_HexTile* GetActiveTile() const { return ActiveTile; }
    
    // ========================================
    // Utility
    // ========================================
    
    /** Get all hex neighbors of a grid position */
    UFUNCTION(BlueprintPure, Category = "Hex Map")
    TArray<FIntPoint> GetHexNeighbors(FIntPoint GridPos) const;
    
    /** Calculate distance between two hex positions */
    UFUNCTION(BlueprintPure, Category = "Hex Map")
    int32 GetHexDistance(FIntPoint A, FIntPoint B) const;
    
protected:
    // ========================================
    // Internal Helpers
    // ========================================
    
    /** Get or create a tile from pool */
    UGW_HexTile* GetTileFromPool();
    
    /** Return tile to pool for reuse */
    void ReturnTileToPool(UGW_HexTile* Tile);
    
    /** Calculate which tiles should be visible */
    TArray<FIntPoint> CalculateVisibleTileRange() const;
};
#pragma endregion
/*-------------------------------------------------------------------------*/