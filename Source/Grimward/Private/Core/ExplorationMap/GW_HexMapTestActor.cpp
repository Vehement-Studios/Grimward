// HexMapTestActor.cpp
#include "Core/ExplorationMap/GW_HexMapTestActor.h"
#include "DrawDebugHelpers.h"

AGW_HexMapTestActor::AGW_HexMapTestActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGW_HexMapTestActor::BeginPlay()
{
    Super::BeginPlay();

    if (bAutoInitialize)
    {
        TestInitializeMap();
        GenerateAndVisualizeArea(0, 0, 10);
        
        if (bShowDebugVisualization)
        {
            VisualizeMap();
        }
    }
}

void AGW_HexMapTestActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShowDebugVisualization && MapGenerator)
    {
        VisualizeMap();
    }
}

void AGW_HexMapTestActor::TestInitializeMap()
{
    if (!MapGenerator)
    {
        MapGenerator = GetWorld()->SpawnActor<AGW_HexMapGenerator>();
    }

    MapGenerator->InitializeMap(TestSeed, FGW_HexCoordinate(0, 0));

    UE_LOG(LogTemp, Warning, TEXT("=== MAP INITIALIZED ==="));
    UE_LOG(LogTemp, Warning, TEXT("Seed: %u"), TestSeed);
    
    PrintMapStatistics();
}

void AGW_HexMapTestActor::TestGenerateTiles()
{
    if (!MapGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("Map not initialized!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("=== GENERATING TEST TILES ==="));

    // Test various coordinates
    TArray<FGW_HexCoordinate> TestCoords = {
        FGW_HexCoordinate(0, 0),
        FGW_HexCoordinate(1, 0),
        FGW_HexCoordinate(0, 1),
        FGW_HexCoordinate(-1, 1),
        FGW_HexCoordinate(2, -1),
        FGW_HexCoordinate(5, 3)
    };

    for (const FGW_HexCoordinate& Coord : TestCoords)
    {
        FGW_HexTileData Tile = MapGenerator->GetTileData(Coord);
        
        UE_LOG(LogTemp, Log, TEXT("Tile %s: %s | POI: %s | Rarity: %s | Unlocked: %d | Conquered: %d"),
            *Coord.ToString(),
            *UEnum::GetDisplayValueAsText(Tile.BiomeType).ToString(),
            *UEnum::GetDisplayValueAsText(Tile.POIType).ToString(),
            *UEnum::GetDisplayValueAsText(Tile.Rarity).ToString(),
            Tile.bIsUnlocked,
            Tile.bIsConquered);
    }
}

void AGW_HexMapTestActor::TestConquerTile(FGW_HexCoordinate Coordinate)
{
    if (!MapGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("Map not initialized!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("=== CONQUERING TILE %s ==="), *Coordinate.ToString());

    int32 BeforeUnlocked = MapGenerator->GetUnlockedTiles().Num();
    
    MapGenerator->ConquerTile(Coordinate, 1);
    
    int32 AfterUnlocked = MapGenerator->GetUnlockedTiles().Num();
    
    UE_LOG(LogTemp, Log, TEXT("Unlocked tiles before: %d"), BeforeUnlocked);
    UE_LOG(LogTemp, Log, TEXT("Unlocked tiles after: %d"), AfterUnlocked);
    UE_LOG(LogTemp, Log, TEXT("New unlocked tiles: %d"), AfterUnlocked - BeforeUnlocked);

    PrintMapStatistics();
}

void AGW_HexMapTestActor::PrintMapStatistics()
{
    if (!MapGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("Map not initialized!"));
        return;
    }

    int32 TotalConquered = MapGenerator->GetConqueredTileCount();
    int32 TotalUnlocked = MapGenerator->GetUnlockedTiles().Num();
    int32 TotalVisible = MapGenerator->GetVisibleTiles().Num();

    UE_LOG(LogTemp, Warning, TEXT("=== MAP STATISTICS ==="));
    UE_LOG(LogTemp, Log, TEXT("Conquered Tiles: %d"), TotalConquered);
    UE_LOG(LogTemp, Log, TEXT("Unlocked Tiles: %d"), TotalUnlocked);
    UE_LOG(LogTemp, Log, TEXT("Visible Tiles: %d"), TotalVisible);
}

void AGW_HexMapTestActor::TestSaveLoad()
{
    if (!MapGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("Map not initialized!"));
        return;
    }

    FString TestSlot = TEXT("TestSave");

    UE_LOG(LogTemp, Warning, TEXT("=== TESTING SAVE/LOAD ==="));

    // Save current state
    bool bSaved = MapGenerator->SaveMapState(TestSlot);
    UE_LOG(LogTemp, Log, TEXT("Save %s"), bSaved ? TEXT("SUCCESS") : TEXT("FAILED"));

    // Store current state
    int32 ConqueredBefore = MapGenerator->GetConqueredTileCount();

    // Modify the map
    MapGenerator->ConquerTile(FGW_HexCoordinate(1, 0), 1);
    int32 ConqueredAfterModify = MapGenerator->GetConqueredTileCount();

    // Load saved state
    bool bLoaded = MapGenerator->LoadMapState(TestSlot);
    UE_LOG(LogTemp, Log, TEXT("Load %s"), bLoaded ? TEXT("SUCCESS") : TEXT("FAILED"));

    int32 ConqueredAfterLoad = MapGenerator->GetConqueredTileCount();

    UE_LOG(LogTemp, Log, TEXT("Conquered before save: %d"), ConqueredBefore);
    UE_LOG(LogTemp, Log, TEXT("Conquered after modify: %d"), ConqueredAfterModify);
    UE_LOG(LogTemp, Log, TEXT("Conquered after load: %d"), ConqueredAfterLoad);
    UE_LOG(LogTemp, Log, TEXT("Load restored correctly: %s"), 
        ConqueredAfterLoad == ConqueredBefore ? TEXT("YES") : TEXT("NO"));
}

void AGW_HexMapTestActor::FindRareBiomes(int32 SearchRadius)
{
    if (!MapGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("Map not initialized!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("=== SEARCHING FOR RARE BIOMES (Radius: %d) ==="), SearchRadius);

    TArray<FGW_HexCoordinate> SearchArea = FGW_HexCoordinate(0, 0).GetHexesInRange(SearchRadius);
    
    TMap<EBiomeType, int32> BiomeCounts;

    for (const FGW_HexCoordinate& Coord : SearchArea)
    {
        FGW_HexTileData Tile = MapGenerator->GetTileData(Coord);
        
        // Count rare biomes
        if (Tile.BiomeType == EBiomeType::MysticForest ||
            Tile.BiomeType == EBiomeType::PoisonSwamp ||
            Tile.BiomeType == EBiomeType::DragonBones ||
            Tile.BiomeType == EBiomeType::Lavascape ||
            Tile.BiomeType == EBiomeType::IceSpikes)
        {
            BiomeCounts.FindOrAdd(Tile.BiomeType)++;
            
            UE_LOG(LogTemp, Log, TEXT("Found %s at %s"), 
                *UEnum::GetDisplayValueAsText(Tile.BiomeType).ToString(),
                *Coord.ToString());
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("=== RARE BIOME SUMMARY ==="));
    for (const auto& Pair : BiomeCounts)
    {
        UE_LOG(LogTemp, Log, TEXT("%s: %d"), 
            *UEnum::GetDisplayValueAsText(Pair.Key).ToString(),
            Pair.Value);
    }
}

void AGW_HexMapTestActor::FindMegagons(int32 SearchRadius)
{
    if (!MapGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("Map not initialized!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("=== SEARCHING FOR MEGAGONS (Radius: %d) ==="), SearchRadius);

    TArray<FGW_HexCoordinate> SearchArea = FGW_HexCoordinate(0, 0).GetHexesInRange(SearchRadius);
    
    int32 MegagonCount = 0;

    for (const FGW_HexCoordinate& Coord : SearchArea)
    {
        FGW_HexTileData Tile = MapGenerator->GetTileData(Coord);
        
        if (Tile.bIsMegagonCenter)
        {
            MegagonCount++;
            
            UE_LOG(LogTemp, Log, TEXT("Found MEGAGON: %s at %s"), 
                *UEnum::GetDisplayValueAsText(Tile.POIType).ToString(),
                *Coord.ToString());
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Total Megagons Found: %d"), MegagonCount);
}

FVector AGW_HexMapTestActor::HexToWorldPosition(const FGW_HexCoordinate& Coord) const
{
    // Flat-top hex layout
    float X = HexSize * (3.0f / 2.0f * Coord.Q);
    float Y = HexSize * (FMath::Sqrt(3.0f) / 2.0f * Coord.Q + FMath::Sqrt(3.0f) * Coord.R);
    
    return GetActorLocation() + FVector(X, Y, 0.0f);
}

void AGW_HexMapTestActor::DrawHexOutline(const FGW_HexCoordinate& Coord, FColor Color, float Thickness) const
{
    if (!GetWorld()) return;

    FVector Center = HexToWorldPosition(Coord);
    
    // Calculate hex vertices (flat-top orientation)
    TArray<FVector> Vertices;
    for (int32 i = 0; i < 6; ++i)
    {
        float AngleDeg = 60.0f * i;
        float AngleRad = FMath::DegreesToRadians(AngleDeg);
        
        FVector Vertex = Center + FVector(
            HexSize * FMath::Cos(AngleRad),
            HexSize * FMath::Sin(AngleRad),
            0.0f
        );
        Vertices.Add(Vertex);
    }

    // Draw lines between vertices
    for (int32 i = 0; i < 6; ++i)
    {
        int32 NextIndex = (i + 1) % 6;
        DrawDebugLine(GetWorld(), Vertices[i], Vertices[NextIndex], 
            Color, false, -1.0f, 0, Thickness);
    }
}

void AGW_HexMapTestActor::VisualizeMap()
{
    if (!MapGenerator || !GetWorld())
    {
        return;
    }

    TArray<FGW_HexCoordinate> VisibleTiles = MapGenerator->GetVisibleTiles();

    for (const FGW_HexCoordinate& Coord : VisibleTiles)
    {
        if (Coord.Distance(FGW_HexCoordinate(0, 0)) > DebugRadius)
            continue;

        FGW_HexTileData Tile = MapGenerator->GetTileData(Coord);
        FVector WorldPos = HexToWorldPosition(Coord);

        // Determine hex outline color and text color based on biome
        FColor HexOutlineColor;
        FColor TextColor;
        
        // Get colors based on biome type
        GetBiomeColors(Tile.BiomeType, HexOutlineColor, TextColor);
        
        // Override for megagons
        if (Tile.bIsPartOfMegagon)
        {
            HexOutlineColor = FColor::Black;
            TextColor = FColor::Black;
        }

        // Draw hex outline
        DrawHexOutline(Coord, HexOutlineColor, 3.0f);

        // Draw center sphere (same color as outline for consistency)
        DrawDebugSphere(GetWorld(), WorldPos, 20.0f, 8, HexOutlineColor, false, -1.0f, 0, 2.0f);

        // Draw coordinate and biome text
        FString InfoText = FString::Printf(TEXT("%s\n%s"), 
            *Coord.ToString(),
            *UEnum::GetDisplayValueAsText(Tile.BiomeType).ToString());
        
        // Add POI info if present
        if (Tile.POIType != EPOIType::None)
        {
            InfoText += FString::Printf(TEXT("\n%s"), 
                *UEnum::GetDisplayValueAsText(Tile.POIType).ToString());
        }
        
        DrawDebugString(GetWorld(), WorldPos + FVector(0, 0, 50), 
            InfoText, nullptr, TextColor, -1.0f, true, 1.0f);
    }
}

void AGW_HexMapTestActor::GetBiomeColors(EBiomeType Biome, FColor& OutlineColor, FColor& TextColor) const
{
    switch (Biome)
    {
        // Basic Biomes
        case EBiomeType::Hills:
            OutlineColor = FColor::Yellow;
            TextColor = FColor::White;
            break;
            
        case EBiomeType::Forest:
            OutlineColor = FColor::Green;
            TextColor = FColor::White;
            break;
            
        case EBiomeType::Mountains:
            OutlineColor = FColor::Blue;
            TextColor = FColor::White;
            break;
            
        case EBiomeType::Desert:
            OutlineColor = FColor::Orange;
            TextColor = FColor::White;
            break;
            
        case EBiomeType::Swamp:
            OutlineColor = FColor(128, 128, 128); // Gray
            TextColor = FColor::White;
            break;
            
        // Rare Biomes
        case EBiomeType::MysticForest:
            OutlineColor = FColor::Green;
            TextColor = FColor(128, 0, 128); // Purple
            break;
            
        case EBiomeType::PoisonSwamp:
            OutlineColor = FColor(128, 128, 128); // Gray
            TextColor = FColor(128, 0, 128); // Purple
            break;
            
        case EBiomeType::DragonBones:
            OutlineColor = FColor::Orange;
            TextColor = FColor(128, 0, 128); // Purple
            break;
            
        case EBiomeType::Lavascape:
            OutlineColor = FColor::Blue;
            TextColor = FColor::Red;
            break;
            
        case EBiomeType::IceSpikes:
            OutlineColor = FColor::Blue;
            TextColor = FColor(173, 216, 230); // Light Blue
            break;
            
        default:
            OutlineColor = FColor::White;
            TextColor = FColor::White;
            break;
    }
}

FColor AGW_HexMapTestActor::GetBiomeColor(EBiomeType Biome) const
{
    // This function is now deprecated but kept for compatibility
    // Use GetBiomeColors instead
    FColor OutlineColor, TextColor;
    GetBiomeColors(Biome, OutlineColor, TextColor);
    return OutlineColor;
}

FColor AGW_HexMapTestActor::GetRarityColor(ETileRarity Rarity) const
{
    switch (Rarity)
    {
        case ETileRarity::Basic: return FColor::White;
        case ETileRarity::Rare: return FColor::Blue;
        case ETileRarity::Epic: return FColor::Purple;
        case ETileRarity::Megagon: return FColor::Orange;
        default: return FColor::White;
    }
}

void AGW_HexMapTestActor::GenerateAndUnlockRadius(int32 Radius)
{
    if (!MapGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("Map not initialized!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("=== GENERATING AND UNLOCKING RADIUS %d ==="), Radius);

    // Use the generator's method to unlock the area
    MapGenerator->ForceUnlockArea(FGW_HexCoordinate(0, 0), Radius);
    
    PrintMapStatistics();
}

void AGW_HexMapTestActor::GenerateAndVisualizeArea(int32 CenterQ, int32 CenterR, int32 Radius)
{
    if (!MapGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("Map not initialized!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("=== GENERATING AREA AT (%d, %d) WITH RADIUS %d ==="), CenterQ, CenterR, Radius);

    FGW_HexCoordinate Center(CenterQ, CenterR);
    
    // Force unlock the area
    MapGenerator->ForceUnlockArea(Center, Radius);
    
    // Collect statistics
    TArray<FGW_HexCoordinate> AllCoords = Center.GetHexesInRange(Radius);
    
    TMap<EBiomeType, int32> BiomeCount;
    TMap<ETileRarity, int32> RarityCount;
    int32 POICount = 0;
    int32 MegagonCount = 0;

    for (const FGW_HexCoordinate& Coord : AllCoords)
    {
        FGW_HexTileData Tile = MapGenerator->GetTileData(Coord);

        BiomeCount.FindOrAdd(Tile.BiomeType)++;
        RarityCount.FindOrAdd(Tile.Rarity)++;
        
        if (Tile.POIType != EPOIType::None)
        {
            POICount++;
        }
        
        if (Tile.bIsMegagonCenter)
        {
            MegagonCount++;
            UE_LOG(LogTemp, Log, TEXT("Found Megagon: %s at %s"), 
                *UEnum::GetDisplayValueAsText(Tile.POIType).ToString(),
                *Coord.ToString());
        }
    }

    // Print statistics
    UE_LOG(LogTemp, Warning, TEXT("=== AREA STATISTICS ==="));
    UE_LOG(LogTemp, Log, TEXT("Center: (%d, %d)"), CenterQ, CenterR);
    UE_LOG(LogTemp, Log, TEXT("Radius: %d"), Radius);
    UE_LOG(LogTemp, Log, TEXT("Total Tiles: %d"), AllCoords.Num());
    UE_LOG(LogTemp, Log, TEXT("Tiles with POIs: %d (%.1f%%)"), POICount, (POICount * 100.0f) / AllCoords.Num());
    UE_LOG(LogTemp, Log, TEXT("Megagons Found: %d"), MegagonCount);

    UE_LOG(LogTemp, Warning, TEXT("=== BIOME DISTRIBUTION ==="));
    for (const auto& Pair : BiomeCount)
    {
        float Percentage = (Pair.Value * 100.0f) / AllCoords.Num();
        UE_LOG(LogTemp, Log, TEXT("%s: %d (%.1f%%)"), 
            *UEnum::GetDisplayValueAsText(Pair.Key).ToString(),
            Pair.Value,
            Percentage);
    }

    UE_LOG(LogTemp, Warning, TEXT("=== RARITY DISTRIBUTION ==="));
    for (const auto& Pair : RarityCount)
    {
        float Percentage = (Pair.Value * 100.0f) / AllCoords.Num();
        UE_LOG(LogTemp, Log, TEXT("%s: %d (%.1f%%)"), 
            *UEnum::GetDisplayValueAsText(Pair.Key).ToString(),
            Pair.Value,
            Percentage);
    }

    // Update debug radius to show the generated area
    DebugRadius = Radius + FMath::Abs(CenterQ) + FMath::Abs(CenterR) + 5; // Add 5 for buffer
    
    UE_LOG(LogTemp, Log, TEXT("Debug visualization updated to radius %d"), DebugRadius);
}
