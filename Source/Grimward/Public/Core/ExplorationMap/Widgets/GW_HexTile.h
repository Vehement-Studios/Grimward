// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/ExplorationMap/GW_TileTypes.h"
#include "GW_HexTile.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/
class UImage;
class UOverlay;
class UButton;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Hex Tile State                                                         */
/*-------------------------------------------------------------------------*/
UENUM(BlueprintType)
enum class EGW_HexTileState : uint8
{
    Hidden      UMETA(DisplayName = "Hidden"),      // Player hasn't explored
    Explored    UMETA(DisplayName = "Explored"),    // Player can see it
    Conquered   UMETA(DisplayName = "Conquered")    // Player defeated it
};

/*-------------------------------------------------------------------------*/
/*  Hex Tile Data                                                          */
/*-------------------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FGW_HexTileData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint GridPosition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGW_HexBiome BiomeType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGW_HexPOI POIType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGW_HexTileState TileState;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> AvailableLoot;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> CollectedLoot;
    
    FGW_HexTileData()
        : GridPosition(FIntPoint::ZeroValue)
        , BiomeType(EGW_HexBiome::Hill)
        , POIType(EGW_HexPOI::None)
        , TileState(EGW_HexTileState::Hidden)
    {
    }
};

/*-------------------------------------------------------------------------*/
/*  Hex Tile Widget                                                        */
/*-------------------------------------------------------------------------*/
#pragma region GW_HexTile.h
UCLASS()
class GRIMWARD_API UGW_HexTile : public UUserWidget
{
    GENERATED_BODY()

protected:
    // ========================================
    // Widget Components (Bind in UMG)
    // ========================================
    
    /** Root overlay for layering */
    UPROPERTY(meta = (BindWidget))
    UOverlay* TileOverlay;
    
    /** Base biome image */
    UPROPERTY(meta = (BindWidget))
    UImage* BiomeImage;
    
    /** Fog of war overlay */
    UPROPERTY(meta = (BindWidget))
    UImage* FogOverlay;
    
    /** Conquered state overlay */
    UPROPERTY(meta = (BindWidget))
    UImage* ConqueredOverlay;
    
    /** POI icon image */
    UPROPERTY(meta = (BindWidgetOptional))
    UImage* TilePOI;
    
    /** Selection/highlight border */
    UPROPERTY(meta = (BindWidgetOptional))
    UImage* SelectionBorder;
    
    /** Clickable button overlay */
    UPROPERTY(meta = (BindWidget))
    UButton* TileButton;
    
    // ========================================
    // Tile Data
    // ========================================
    
    UPROPERTY(BlueprintReadOnly, Category = "Hex Tile")
    FGW_HexTileData TileData;
    
    UPROPERTY(BlueprintReadOnly, Category = "Hex Tile")
    FVector2D PixelPosition;
    
    UPROPERTY(BlueprintReadOnly, Category = "Hex Tile")
    bool bIsSelected;
    
public:
    // ========================================
    // Initialization
    // ========================================
    
    virtual void NativeConstruct() override;
    
    /** Initialize tile with data from map generator */
    UFUNCTION(BlueprintCallable, Category = "Hex Tile")
    void InitializeTile(const FGW_HexTileData& InTileData, FVector2D InPixelPosition);
    
    // ========================================
    // State Management
    // ========================================
    
    /** Update tile visual state */
    UFUNCTION(BlueprintCallable, Category = "Hex Tile")
    void SetTileState(EGW_HexTileState NewState);
    
    /** Update the biome texture */
    UFUNCTION(BlueprintCallable, Category = "Hex Tile")
    void UpdateBiomeVisuals(EGW_HexBiome Biome);
    
    /** Update POI icon */
    UFUNCTION(BlueprintCallable, Category = "Hex Tile")
    void UpdatePOIVisuals(EGW_HexPOI POI);
    
    /** Toggle selection highlight */
    UFUNCTION(BlueprintCallable, Category = "Hex Tile")
    void SetSelected(bool bSelected);
    
    // ========================================
    // Input Handling
    // ========================================
    
    UFUNCTION()
    void OnTileClicked();
    
    UFUNCTION()
    void OnTileHovered();
    
    UFUNCTION()
    void OnTileUnhovered();
    
    // ========================================
    // Accessors
    // ========================================
    
    UFUNCTION(BlueprintPure, Category = "Hex Tile")
    FIntPoint GetGridPosition() const { return TileData.GridPosition; }
    
    UFUNCTION(BlueprintPure, Category = "Hex Tile")
    EGW_HexTileState GetTileState() const { return TileData.TileState; }
    
    UFUNCTION(BlueprintPure, Category = "Hex Tile")
    FVector2D GetPixelPosition() const { return PixelPosition; }
    
    // ========================================
    // Blueprint Events
    // ========================================
    
    /** Called when tile is clicked - implement in BP for animations */
    UFUNCTION(BlueprintImplementableEvent, Category = "Hex Tile")
    void BP_OnTileClicked();
    
    /** Called when tile state changes - implement in BP for transitions */
    UFUNCTION(BlueprintImplementableEvent, Category = "Hex Tile")
    void BP_OnStateChanged(EGW_HexTileState NewState);
};
#pragma endregion
/*-------------------------------------------------------------------------*/