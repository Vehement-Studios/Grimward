// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Core/ExplorationMap/GW_ExplorableHexMap.h"
#include "Core/ExplorationMap/Widgets/GW_HexTile.h"
#include "Core/ExplorationMap/GW_MapGenerator.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Grimward.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_ExplorableHexMap.cpp
void UGW_ExplorableHexMap::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Initialize state
    ViewportOffset = FVector2D::ZeroVector;
    CurrentZoom = 1.0f;
    bIsDragging = false;
    LastCenterGridPos = FIntPoint(0, 0);
    ActiveTile = nullptr;
    
    UE_LOG(LogGrimward, Log, TEXT("GW_ExplorableHexMap constructed"));
}

void UGW_ExplorableHexMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // Could add smooth camera interpolation here if needed
}

void UGW_ExplorableHexMap::InitializeMap(AGW_MapGenerator* InMapGenerator, int32 MapSeed, FIntPoint StartingGridPos)
{
    if (!InMapGenerator)
    {
        UE_LOG(LogGrimward, Error, TEXT("InitializeMap called with null MapGenerator!"));
        return;
    }
    
    MapGenerator = InMapGenerator;
    CurrentMapSeed = MapSeed;
    
    // Generate the biome map if not already generated
    if (MapGenerator->GetBiomeMap().Num() == 0)
    {
        MapGenerator->GenerateBiomeMap(MapSeed);
    }
    
    // Center on starting position
    CenterOnGridPosition(StartingGridPos);
    
    // Populate initial tiles
    UpdateVisibleTiles();
    
    UE_LOG(LogGrimward, Log, TEXT("Map initialized with seed %d at grid position (%d, %d)"),
        MapSeed, StartingGridPos.X, StartingGridPos.Y);
}

void UGW_ExplorableHexMap::InitializeMapAtOrigin(AGW_MapGenerator* InMapGenerator, int32 MapSeed)
{
    InitializeMap(InMapGenerator, MapSeed, FIntPoint(0, 0));
}

UGW_HexTile* UGW_ExplorableHexMap::BuildHexTile(FIntPoint GridPos, int32 MapSeed)
{
    if (!MapGenerator)
    {
        UE_LOG(LogGrimward, Error, TEXT("BuildHexTile: MapGenerator is null!"));
        return nullptr;
    }
    
    // Get tile from pool or create new
    UGW_HexTile* Tile = GetTileFromPool();
    if (!Tile)
    {
        UE_LOG(LogGrimward, Error, TEXT("BuildHexTile: Failed to get tile from pool!"));
        return nullptr;
    }
    
    // Get biome data from generator
    FGW_BiomeData BiomeData = MapGenerator->GetBiomeDataAt(GridPos.X, GridPos.Y);
    
    // Build tile data
    FGW_HexTileData TileData;
    TileData.GridPosition = GridPos;
    TileData.BiomeType = BiomeData.BiomeEntry;
    TileData.POIType = EGW_HexPOI::None; // TODO: Implement POI generation
    TileData.TileState = EGW_HexTileState::Hidden; // TODO: Get from player data
    
    // Calculate pixel position
    FVector2D PixelPos = GridToPixel(GridPos);
    
    // Initialize the tile
    Tile->InitializeTile(TileData, PixelPos);
    
    return Tile;
}

void UGW_ExplorableHexMap::AddTileToScreen(UGW_HexTile* Tile)
{
    if (!Tile || !MapCanvas)
    {
        UE_LOG(LogGrimward, Error, TEXT("AddTileToScreen: Invalid tile or canvas!"));
        return;
    }
    
    // Add to canvas if not already added
    if (!Tile->IsInViewport())
    {
        UCanvasPanelSlot* CanvasSlot = MapCanvas->AddChildToCanvas(Tile);
        
        if (CanvasSlot)
        {
            // Set size
            CanvasSlot->SetSize(FVector2D(MapConfig.HexWidth, MapConfig.HexHeight));
            
            // Set position accounting for viewport offset and zoom
            FVector2D ScreenPos = GetScreenPosition(Tile->GetPixelPosition());
            CanvasSlot->SetPosition(ScreenPos);
            
            // Set anchor to top-left
            CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 0.0f, 0.0f));
        }
    }
    
    // Track spawned tile
    SpawnedTiles.Add(Tile->GetGridPosition(), Tile);
}

void UGW_ExplorableHexMap::RemoveTileFromScreen(UGW_HexTile* Tile)
{
    if (!Tile)
        return;
        
    // Remove from tracking
    SpawnedTiles.Remove(Tile->GetGridPosition());
    
    // Remove from canvas
    if (Tile->IsInViewport())
    {
        Tile->RemoveFromParent();
    }
    
    // Return to pool
    ReturnTileToPool(Tile);
}

void UGW_ExplorableHexMap::UpdateVisibleTiles()
{
    if (!MapGenerator || !MapCanvas)
        return;
    
    // Calculate which tiles should be visible
    TArray<FIntPoint> VisibleTilePositions = CalculateVisibleTileRange();
    
    // Track which tiles we need to keep
    TSet<FIntPoint> NeededTiles;
    for (const FIntPoint& Pos : VisibleTilePositions)
    {
        NeededTiles.Add(Pos);
    }
    
    // Remove tiles that are no longer visible
    TArray<FIntPoint> TilesToRemove;
    for (const auto& Pair : SpawnedTiles)
    {
        if (!NeededTiles.Contains(Pair.Key))
        {
            TilesToRemove.Add(Pair.Key);
        }
    }
    
    for (const FIntPoint& Pos : TilesToRemove)
    {
        if (UGW_HexTile** TilePtr = SpawnedTiles.Find(Pos))
        {
            RemoveTileFromScreen(*TilePtr);
        }
    }
    
    // Add tiles that are newly visible
    for (const FIntPoint& Pos : VisibleTilePositions)
    {
        if (!SpawnedTiles.Contains(Pos))
        {
            UGW_HexTile* NewTile = BuildHexTile(Pos, CurrentMapSeed);
            if (NewTile)
            {
                AddTileToScreen(NewTile);
            }
        }
        else
        {
            // Update existing tile position for zoom/pan
            if (UGW_HexTile** ExistingTile = SpawnedTiles.Find(Pos))
            {
                if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>((*ExistingTile)->Slot))
                {
                    FVector2D ScreenPos = GetScreenPosition((*ExistingTile)->GetPixelPosition());
                    CanvasSlot->SetPosition(ScreenPos);
                }
            }
        }
    }
    
    UE_LOG(LogGrimward, Log, TEXT("UpdateVisibleTiles: %d tiles visible, %d tiles spawned"),
        VisibleTilePositions.Num(), SpawnedTiles.Num());
}

// ========================================
// Coordinate Conversion
// ========================================

FVector2D UGW_ExplorableHexMap::GridToPixel(FIntPoint GridPos) const
{
    // Axial to pixel conversion for flat-topped hexagons
    // Using offset coordinates (odd-r)
    const float HexWidth = MapConfig.HexWidth;
    const float HexHeight = MapConfig.HexHeight;
    
    float X = GridPos.X * HexWidth * 0.75f;
    float Y = GridPos.Y * HexHeight + (GridPos.X % 2) * (HexHeight * 0.5f);
    
    return FVector2D(X, Y);
}

FIntPoint UGW_ExplorableHexMap::PixelToGrid(FVector2D PixelPos) const
{
    // Reverse conversion (approximate - for click detection you'd want more precision)
    const float HexWidth = MapConfig.HexWidth;
    const float HexHeight = MapConfig.HexHeight;
    
    int32 X = FMath::RoundToInt(PixelPos.X / (HexWidth * 0.75f));
    int32 Y = FMath::RoundToInt((PixelPos.Y - (X % 2) * (HexHeight * 0.5f)) / HexHeight);
    
    return FIntPoint(X, Y);
}

FVector2D UGW_ExplorableHexMap::GetScreenPosition(FVector2D WorldPixelPos) const
{
    return (WorldPixelPos + ViewportOffset) * CurrentZoom;
}

// ========================================
// Camera Control
// ========================================

void UGW_ExplorableHexMap::PanViewport(FVector2D Delta)
{
    ViewportOffset += Delta;
    UpdateVisibleTiles();
}

void UGW_ExplorableHexMap::SetZoom(float NewZoom)
{
    CurrentZoom = FMath::Clamp(NewZoom, MapConfig.ZoomRange.X, MapConfig.ZoomRange.Y);
    UpdateVisibleTiles();
}

void UGW_ExplorableHexMap::CenterOnGridPosition(FIntPoint GridPos)
{
    FVector2D PixelPos = GridToPixel(GridPos);
    
    // TODO: Get actual viewport size instead of hardcoded value
    FVector2D ViewportCenter = FVector2D(960.0f, 540.0f); // 1920x1080 / 2
    
    ViewportOffset = ViewportCenter - PixelPos * CurrentZoom;
    LastCenterGridPos = GridPos;
    
    UpdateVisibleTiles();
}

// ========================================
// Input Handling
// ========================================

FReply UGW_ExplorableHexMap::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        bIsDragging = true;
        LastDragPosition = InMouseEvent.GetScreenSpacePosition();
        return FReply::Handled().CaptureMouse(TakeWidget());
    }
    
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UGW_ExplorableHexMap::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        bIsDragging = false;
        return FReply::Handled().ReleaseMouseCapture();
    }
    
    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UGW_ExplorableHexMap::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging)
    {
        FVector2D CurrentPosition = InMouseEvent.GetScreenSpacePosition();
        FVector2D Delta = CurrentPosition - LastDragPosition;
        
        PanViewport(Delta);
        
        LastDragPosition = CurrentPosition;
        return FReply::Handled();
    }
    
    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UGW_ExplorableHexMap::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    float WheelDelta = InMouseEvent.GetWheelDelta();
    float ZoomDelta = WheelDelta * 0.1f;
    
    SetZoom(CurrentZoom + ZoomDelta);
    
    return FReply::Handled();
}

// ========================================
// Tile Selection
// ========================================

void UGW_ExplorableHexMap::SetActiveTile(UGW_HexTile* Tile)
{
    // Deselect previous active tile
    if (ActiveTile && ActiveTile != Tile)
    {
        ActiveTile->SetSelected(false);
    }
    
    ActiveTile = Tile;
    
    if (ActiveTile)
    {
        ActiveTile->SetSelected(true);
        UE_LOG(LogGrimward, Log, TEXT("Active tile set to grid position (%d, %d)"),
            ActiveTile->GetGridPosition().X, ActiveTile->GetGridPosition().Y);
    }
}

// ========================================
// Utility
// ========================================

TArray<FIntPoint> UGW_ExplorableHexMap::GetHexNeighbors(FIntPoint GridPos) const
{
    TArray<FIntPoint> Neighbors;
    
    // Axial coordinate neighbors for flat-topped hexagons
    static const TArray<FIntPoint> EvenRowOffsets = {
        FIntPoint(1, 0), FIntPoint(0, -1), FIntPoint(-1, -1),
        FIntPoint(-1, 0), FIntPoint(-1, 1), FIntPoint(0, 1)
    };
    
    static const TArray<FIntPoint> OddRowOffsets = {
        FIntPoint(1, 0), FIntPoint(1, -1), FIntPoint(0, -1),
        FIntPoint(-1, 0), FIntPoint(0, 1), FIntPoint(1, 1)
    };
    
    const TArray<FIntPoint>& Offsets = (GridPos.X % 2 == 0) ? EvenRowOffsets : OddRowOffsets;
    
    for (const FIntPoint& Offset : Offsets)
    {
        Neighbors.Add(GridPos + Offset);
    }
    
    return Neighbors;
}

int32 UGW_ExplorableHexMap::GetHexDistance(FIntPoint A, FIntPoint B) const
{
    // Simplified distance calculation for offset coordinates
    int32 DX = FMath::Abs(A.X - B.X);
    int32 DY = FMath::Abs(A.Y - B.Y);
    
    return DX + FMath::Max(0, (DY - DX) / 2);
}

// ========================================
// Internal Helpers
// ========================================

UGW_HexTile* UGW_ExplorableHexMap::GetTileFromPool()
{
    // Try to get from pool first
    if (TilePool.Num() > 0)
    {
        return TilePool.Pop();
    }
    
    // Create new tile if pool is empty
    if (HexTileClass)
    {
        return CreateWidget<UGW_HexTile>(GetOwningPlayer(), HexTileClass);
    }
    
    UE_LOG(LogGrimward, Error, TEXT("GetTileFromPool: HexTileClass not set!"));
    return nullptr;
}

void UGW_ExplorableHexMap::ReturnTileToPool(UGW_HexTile* Tile)
{
    if (Tile)
    {
        TilePool.Add(Tile);
    }
}

TArray<FIntPoint> UGW_ExplorableHexMap::CalculateVisibleTileRange() const
{
    TArray<FIntPoint> VisibleTiles;
    
    if (!MapGenerator)
        return VisibleTiles;
    
    // TODO: Calculate actual viewport bounds instead of using hardcoded values
    FVector2D ViewportSize(1920.0f, 1080.0f);
    
    // Calculate grid range that's visible
    FVector2D TopLeft = PixelToGrid(-ViewportOffset / CurrentZoom);
    FVector2D BottomRight = PixelToGrid((ViewportSize - ViewportOffset) / CurrentZoom);
    
    int32 MinX = FMath::Max(0, (int32)TopLeft.X - MapConfig.TileRenderBuffer);
    int32 MaxX = FMath::Min(MapGenerator->GenWidth - 1, (int32)BottomRight.X + MapConfig.TileRenderBuffer);
    int32 MinY = FMath::Max(0, (int32)TopLeft.Y - MapConfig.TileRenderBuffer);
    int32 MaxY = FMath::Min(MapGenerator->GenHeight - 1, (int32)BottomRight.Y + MapConfig.TileRenderBuffer);
    
    for (int32 X = MinX; X <= MaxX; ++X)
    {
        for (int32 Y = MinY; Y <= MaxY; ++Y)
        {
            VisibleTiles.Add(FIntPoint(X, Y));
        }
    }
    
    return VisibleTiles;
}
#pragma endregion
/*-------------------------------------------------------------------------*/