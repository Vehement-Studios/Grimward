// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Core/ExplorationMap/Widgets/GW_HexTile.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Grimward.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_HexTile.cpp
void UGW_HexTile::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Bind button events
    if (TileButton)
    {
        TileButton->OnClicked.AddDynamic(this, &UGW_HexTile::OnTileClicked);
        TileButton->OnHovered.AddDynamic(this, &UGW_HexTile::OnTileHovered);
        TileButton->OnUnhovered.AddDynamic(this, &UGW_HexTile::OnTileUnhovered);
    }
    
    // Initialize as hidden by default
    bIsSelected = false;
    if (SelectionBorder)
    {
        SelectionBorder->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UGW_HexTile::InitializeTile(const FGW_HexTileData& InTileData, FVector2D InPixelPosition)
{
    TileData = InTileData;
    PixelPosition = InPixelPosition;
    
    // Update visuals based on tile data
    UpdateBiomeVisuals(TileData.BiomeType);
    UpdatePOIVisuals(TileData.POIType);
    SetTileState(TileData.TileState);
    
    UE_LOG(LogGrimward, Log, TEXT("Initialized HexTile at grid position (%d, %d), pixel position (%.1f, %.1f)"),
        TileData.GridPosition.X, TileData.GridPosition.Y, PixelPosition.X, PixelPosition.Y);
}

void UGW_HexTile::SetTileState(EGW_HexTileState NewState)
{
    TileData.TileState = NewState;
    
    // Update fog overlay visibility
    if (FogOverlay)
    {
        switch (NewState)
        {
            case EGW_HexTileState::Hidden:
                FogOverlay->SetVisibility(ESlateVisibility::Visible);
                FogOverlay->SetOpacity(1.0f);
                break;
                
            case EGW_HexTileState::Explored:
                FogOverlay->SetVisibility(ESlateVisibility::Visible);
                FogOverlay->SetOpacity(0.3f); // Semi-transparent
                break;
                
            case EGW_HexTileState::Conquered:
                FogOverlay->SetVisibility(ESlateVisibility::Collapsed);
                break;
        }
    }
    
    // Update conquered overlay
    if (ConqueredOverlay)
    {
        ConqueredOverlay->SetVisibility(
            NewState == EGW_HexTileState::Conquered ? 
            ESlateVisibility::Visible : 
            ESlateVisibility::Collapsed
        );
    }
    
    // Call blueprint event for custom animations
    BP_OnStateChanged(NewState);
}

void UGW_HexTile::UpdateBiomeVisuals(EGW_HexBiome Biome)
{
    // TODO: Load actual biome textures from data asset
    // For now this is a placeholder
    if (BiomeImage)
    {
        // You'll implement texture loading from your data asset here
        UE_LOG(LogGrimward, Log, TEXT("UpdateBiomeVisuals called for biome type %d"), (int32)Biome);
    }
}

void UGW_HexTile::UpdatePOIVisuals(EGW_HexPOI POI)
{
    if (!TilePOI)
        return;
        
    if (POI == EGW_HexPOI::None)
    {
        TilePOI->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        TilePOI->SetVisibility(ESlateVisibility::Visible);
        // TODO: Load actual POI icon textures from data asset
        UE_LOG(LogGrimward, Log, TEXT("UpdatePOIVisuals called for POI type %d"), (int32)POI);
    }
}

void UGW_HexTile::SetSelected(bool bSelected)
{
    bIsSelected = bSelected;
    
    if (SelectionBorder)
    {
        SelectionBorder->SetVisibility(
            bSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
        );
    }
}

void UGW_HexTile::OnTileClicked()
{
    UE_LOG(LogGrimward, Log, TEXT("Tile clicked at grid position (%d, %d)"), 
        TileData.GridPosition.X, TileData.GridPosition.Y);
    
    // Call blueprint event for custom click handling
    BP_OnTileClicked();
}

void UGW_HexTile::OnTileHovered()
{
    // Can add hover effects here
}

void UGW_HexTile::OnTileUnhovered()
{
    // Can remove hover effects here
}
#pragma endregion
/*-------------------------------------------------------------------------*/