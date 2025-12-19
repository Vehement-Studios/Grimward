// GW_HexMapGenerator.cpp
#include "Core/ExplorationMap/GW_HexMapGenerator.h"

AGW_HexMapGenerator::AGW_HexMapGenerator()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGW_HexMapGenerator::BeginPlay()
{
    Super::BeginPlay();
    InitializePOISpawnRules();
}

void AGW_HexMapGenerator::InitializeMap(int32 InSeed, FGW_HexCoordinate StartingCoordinate)
{
    MapSeed = InSeed;
    RandomGenerator.SetSeed(InSeed);
    
    GeneratedTiles.Empty();
    MegagonCenters.Empty();

    // Generate the starting cluster of 7 tiles
    TArray<FGW_HexCoordinate> StartingCluster = StartingCoordinate.GetMegagonCluster();
    
    for (const FGW_HexCoordinate& Coord : StartingCluster)
    {
        FGW_HexTileData TileData = GenerateTile(Coord);
        TileData.bIsUnlocked = true;
        TileData.bIsVisible = true;
        GeneratedTiles.Add(Coord, TileData);
    }

    // Mark the starting tile as conquered
    if (FGW_HexTileData* StartTile = GeneratedTiles.Find(StartingCoordinate))
    {
        StartTile->bIsConquered = true;
    }

    UE_LOG(LogTemp, Log, TEXT("GW_Hex Map Initialized with seed: %u"), MapSeed);
}

FGW_HexTileData AGW_HexMapGenerator::GetTileData(const FGW_HexCoordinate& Coordinate)
{
    // If tile already exists, return it
    if (FGW_HexTileData* ExistingTile = GeneratedTiles.Find(Coordinate))
    {
        return *ExistingTile;
    }

    // Otherwise generate it
    FGW_HexTileData NewTile = GenerateTile(Coordinate);
    GeneratedTiles.Add(Coordinate, NewTile);
    return NewTile;
}

bool AGW_HexMapGenerator::DoesTileExist(const FGW_HexCoordinate& Coordinate) const
{
    return GeneratedTiles.Contains(Coordinate);
}

void AGW_HexMapGenerator::ConquerTile(const FGW_HexCoordinate& Coordinate, int32 ViewRange)
{
    if (FGW_HexTileData* Tile = GeneratedTiles.Find(Coordinate))
    {
        if (!Tile->bIsUnlocked)
        {
            UE_LOG(LogTemp, Warning, TEXT("Attempting to conquer locked tile at %s"), *Coordinate.ToString());
            return;
        }

        Tile->bIsConquered = true;
        
        // Unlock adjacent tiles
        UnlockAdjacentTiles(Coordinate);
        
        // Update visibility
        UpdateVisibility(ViewRange);

        UE_LOG(LogTemp, Log, TEXT("Conquered tile at %s - %s"), 
            *Coordinate.ToString(), *Tile->GetDisplayName());
    }
}

void AGW_HexMapGenerator::UnlockAdjacentTiles(const FGW_HexCoordinate& Coordinate)
{
    TArray<FGW_HexCoordinate> Neighbors = Coordinate.GetNeighbors();
    
    for (const FGW_HexCoordinate& NeighborCoord : Neighbors)
    {
        // Get or generate the tile
        FGW_HexTileData TileData = GetTileData(NeighborCoord);
        
        // Unlock it
        TileData.bIsUnlocked = true;
        
        // Update in map
        GeneratedTiles.Add(NeighborCoord, TileData);
    }
}

void AGW_HexMapGenerator::UpdateVisibility(int32 ViewRange)
{
    // First, mark all tiles as not visible
    for (auto& Pair : GeneratedTiles)
    {
        Pair.Value.bIsVisible = false;
    }

    // Then mark tiles within view range of conquered tiles as visible
    TArray<FGW_HexCoordinate> ConqueredCoords = GetConqueredTiles();
    
    for (const FGW_HexCoordinate& ConqueredCoord : ConqueredCoords)
    {
        TArray<FGW_HexCoordinate> VisibleCoords = ConqueredCoord.GetHexesInRange(ViewRange);
        
        for (const FGW_HexCoordinate& VisibleCoord : VisibleCoords)
        {
            // Generate tile if it doesn't exist
            if (!DoesTileExist(VisibleCoord))
            {
                FGW_HexTileData NewTile = GenerateTile(VisibleCoord);
                GeneratedTiles.Add(VisibleCoord, NewTile);
            }

            // Mark as visible
            if (FGW_HexTileData* Tile = GeneratedTiles.Find(VisibleCoord))
            {
                Tile->bIsVisible = true;
            }
        }
    }
}

TArray<FGW_HexCoordinate> AGW_HexMapGenerator::GetConqueredTiles() const
{
    TArray<FGW_HexCoordinate> Result;
    for (const auto& Pair : GeneratedTiles)
    {
        if (Pair.Value.bIsConquered)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<FGW_HexCoordinate> AGW_HexMapGenerator::GetUnlockedTiles() const
{
    TArray<FGW_HexCoordinate> Result;
    for (const auto& Pair : GeneratedTiles)
    {
        if (Pair.Value.bIsUnlocked)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<FGW_HexCoordinate> AGW_HexMapGenerator::GetVisibleTiles() const
{
    TArray<FGW_HexCoordinate> Result;
    for (const auto& Pair : GeneratedTiles)
    {
        if (Pair.Value.bIsVisible)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

int32 AGW_HexMapGenerator::GetConqueredTileCount() const
{
    int32 Count = 0;
    for (const auto& Pair : GeneratedTiles)
    {
        if (Pair.Value.bIsConquered)
        {
            Count++;
        }
    }
    return Count;
}

// ===== GENERATION METHODS =====

FGW_HexTileData AGW_HexMapGenerator::GenerateTile(const FGW_HexCoordinate& Coordinate)
{
    UE_LOG(LogTemp, Warning, TEXT("=== GenerateTile START: %s ==="), *Coordinate.ToString());
    
    FGW_HexTileData TileData;
    TileData.Coordinate = Coordinate;

    // Check if this tile is part of an existing megagon
    UE_LOG(LogTemp, Log, TEXT("Checking megagon centers (count: %d)"), MegagonCenters.Num());
    for (const FGW_HexCoordinate& MegagonCenter : MegagonCenters)
    {
        if (MegagonCenter.Distance(Coordinate) <= 1)
        {
            UE_LOG(LogTemp, Log, TEXT("Tile is part of megagon centered at %s"), *MegagonCenter.ToString());
            TileData.bIsPartOfMegagon = true;
            TileData.MegagonCenter = MegagonCenter;
            
            if (Coordinate == MegagonCenter)
            {
                TileData.bIsMegagonCenter = true;
            }
            
            TileData.BiomeType = EBiomeType::None;
            TileData.Rarity = ETileRarity::Megagon;
            return TileData;
        }
    }

    // Generate biome
    UE_LOG(LogTemp, Log, TEXT("Generating biome..."));
    TileData.BiomeType = GenerateBiome(Coordinate);
    UE_LOG(LogTemp, Log, TEXT("Generated biome: %s"), *UEnum::GetDisplayValueAsText(TileData.BiomeType).ToString());
    
    // Try to upgrade to rare biome
    UE_LOG(LogTemp, Log, TEXT("Trying rare biome upgrade..."));
    TileData.BiomeType = TryUpgradeToRareBiome(TileData.BiomeType, Coordinate);
    UE_LOG(LogTemp, Log, TEXT("Final biome after upgrade: %s"), *UEnum::GetDisplayValueAsText(TileData.BiomeType).ToString());

    // Try to spawn megagon first
    UE_LOG(LogTemp, Log, TEXT("Trying megagon spawn..."));
    EPOIType MegagonPOI = TrySpawnMegagon(Coordinate, TileData.BiomeType);
    UE_LOG(LogTemp, Log, TEXT("Megagon POI result: %s"), *UEnum::GetDisplayValueAsText(MegagonPOI).ToString());
    
    if (MegagonPOI != EPOIType::None)
    {
        TileData.POIType = MegagonPOI;
        TileData.Rarity = ETileRarity::Megagon;
        TileData.bIsMegagonCenter = true;
        MegagonCenters.Add(Coordinate);
    }
    else
    {
        // Generate regular POI
        UE_LOG(LogTemp, Log, TEXT("Generating regular POI..."));
        TileData.POIType = GeneratePOI(Coordinate, TileData.BiomeType);
        UE_LOG(LogTemp, Log, TEXT("Generated POI: %s"), *UEnum::GetDisplayValueAsText(TileData.POIType).ToString());
        
        TileData.Rarity = DetermineTileRarity(TileData.BiomeType, TileData.POIType);
        UE_LOG(LogTemp, Log, TEXT("Determined rarity: %s"), *UEnum::GetDisplayValueAsText(TileData.Rarity).ToString());
    }

    // Generate visual variant
    UE_LOG(LogTemp, Log, TEXT("Generating visual variant..."));
    TileData.VisualVariant = GenerateVisualVariant(Coordinate, TileData.BiomeType);
    UE_LOG(LogTemp, Log, TEXT("Visual variant: %d"), TileData.VisualVariant);

    // Calculate difficulty
    int32 DistanceFromOrigin = Coordinate.Distance(FGW_HexCoordinate(0, 0));
    TileData.DifficultyLevel = FMath::Max(1, DistanceFromOrigin / 3);

    UE_LOG(LogTemp, Warning, TEXT("=== GenerateTile END: %s - Biome: %s, POI: %s, Rarity: %s ==="), 
        *Coordinate.ToString(),
        *UEnum::GetDisplayValueAsText(TileData.BiomeType).ToString(),
        *UEnum::GetDisplayValueAsText(TileData.POIType).ToString(),
        *UEnum::GetDisplayValueAsText(TileData.Rarity).ToString());

    return TileData;
}

EBiomeType AGW_HexMapGenerator::GenerateBiome(const FGW_HexCoordinate& Coordinate)
{
    UE_LOG(LogTemp, Log, TEXT("  GenerateBiome for %s"), *Coordinate.ToString());
    
    FGW_SeededRandom CoordRandom = RandomGenerator.FromCoordinate(Coordinate.Q, Coordinate.R);

    TArray<EBiomeType> PossibleBiomes = {
        EBiomeType::Hills,
        EBiomeType::Forest,
        EBiomeType::Mountains,
        EBiomeType::Desert,
        EBiomeType::Swamp
    };

    TArray<float> Weights = {
        GenerationConfig.HillsWeight,
        GenerationConfig.ForestWeight,
        GenerationConfig.MountainsWeight,
        GenerationConfig.DesertWeight,
        GenerationConfig.SwampWeight
    };

    UE_LOG(LogTemp, Log, TEXT("  PossibleBiomes: %d, Weights: %d"), PossibleBiomes.Num(), Weights.Num());

    // Apply clustering
    for (int32 i = 0; i < PossibleBiomes.Num(); ++i)
    {
        UE_LOG(LogTemp, Log, TEXT("  Applying clustering for biome index %d: %s"), 
            i, *UEnum::GetDisplayValueAsText(PossibleBiomes[i]).ToString());
        
        float ClusterWeight = GetBiomeClusteringWeight(Coordinate, PossibleBiomes[i]);
        
        UE_LOG(LogTemp, Log, TEXT("  ClusterWeight: %f, Original Weight: %f"), ClusterWeight, Weights[i]);
        
        Weights[i] *= (1.0f + ClusterWeight * GenerationConfig.BiomeClusteringStrength);
        
        UE_LOG(LogTemp, Log, TEXT("  Final Weight: %f"), Weights[i]);
    }

    // Weighted random selection
    float TotalWeight = 0.0f;
    for (float Weight : Weights)
    {
        TotalWeight += Weight;
    }

    UE_LOG(LogTemp, Log, TEXT("  TotalWeight: %f"), TotalWeight);

    float RandomValue = CoordRandom.RandFloatRange(0.0f, TotalWeight);
    UE_LOG(LogTemp, Log, TEXT("  RandomValue: %f"), RandomValue);
    
    float CurrentWeight = 0.0f;

    for (int32 i = 0; i < PossibleBiomes.Num(); ++i)
    {
        CurrentWeight += Weights[i];
        UE_LOG(LogTemp, Log, TEXT("  Checking index %d, CurrentWeight: %f"), i, CurrentWeight);
        
        if (RandomValue <= CurrentWeight)
        {
            UE_LOG(LogTemp, Log, TEXT("  Selected biome: %s"), *UEnum::GetDisplayValueAsText(PossibleBiomes[i]).ToString());
            return PossibleBiomes[i];
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("  Fell through to fallback!"));
    return EBiomeType::Hills;
}

EBiomeType AGW_HexMapGenerator::TryUpgradeToRareBiome(EBiomeType BaseBiome, const FGW_HexCoordinate& Coordinate)
{
    FGW_SeededRandom CoordRandom = RandomGenerator.FromCoordinate(Coordinate.Q, Coordinate.R);

    switch (BaseBiome)
    {
        case EBiomeType::Forest:
            if (CoordRandom.RandBool(GenerationConfig.MysticForestChance))
            {
                return EBiomeType::MysticForest;
            }
            break;

        case EBiomeType::Swamp:
            if (CoordRandom.RandBool(GenerationConfig.PoisonSwampChance))
            {
                return EBiomeType::PoisonSwamp;
            }
            break;

        case EBiomeType::Desert:
            if (CoordRandom.RandBool(GenerationConfig.DragonBonesChance))
            {
                return EBiomeType::DragonBones;
            }
            break;

        case EBiomeType::Mountains:
        {
            float Roll = CoordRandom.RandFloat();
            if (Roll < GenerationConfig.LavascapeChance)
            {
                return EBiomeType::Lavascape;
            }
            else if (Roll < GenerationConfig.LavascapeChance + GenerationConfig.IceSpikesChance)
            {
                return EBiomeType::IceSpikes;
            }
            break;
        }

        default:
            break;
    }

    return BaseBiome;
}

EPOIType AGW_HexMapGenerator::GeneratePOI(const FGW_HexCoordinate& Coordinate, EBiomeType Biome)
{
    UE_LOG(LogTemp, Log, TEXT("  GeneratePOI for %s, Biome: %s"), 
        *Coordinate.ToString(), 
        *UEnum::GetDisplayValueAsText(Biome).ToString());
    
    FGW_SeededRandom CoordRandom = RandomGenerator.FromCoordinate(Coordinate.Q, Coordinate.R);

    // Roll for rare POI
    float RareRoll = CoordRandom.RandFloat();
    UE_LOG(LogTemp, Log, TEXT("  Rare POI roll: %f (threshold: %f)"), RareRoll, GenerationConfig.RarePOIChance);
    
    if (RareRoll < GenerationConfig.RarePOIChance)
    {
        TArray<EPOIType> ValidRarePOIs = GetValidPOIsForBiome(Biome, ETileRarity::Rare);
        UE_LOG(LogTemp, Log, TEXT("  Valid Rare POIs: %d"), ValidRarePOIs.Num());
        
        if (ValidRarePOIs.Num() > 0)
        {
            int32 Index = CoordRandom.RandRange(0, ValidRarePOIs.Num() - 1);
            UE_LOG(LogTemp, Log, TEXT("  Selected rare POI index: %d / %d"), Index, ValidRarePOIs.Num() - 1);
            
            EPOIType SelectedPOI = ValidRarePOIs[Index];
            UE_LOG(LogTemp, Log, TEXT("  Selected rare POI: %s"), *UEnum::GetDisplayValueAsText(SelectedPOI).ToString());

            // Roll for epic upgrade
            float EpicRoll = CoordRandom.RandFloat();
            UE_LOG(LogTemp, Log, TEXT("  Epic POI roll: %f (threshold: %f)"), EpicRoll, GenerationConfig.EpicPOIChance);
            
            if (EpicRoll < GenerationConfig.EpicPOIChance)
            {
                TArray<EPOIType> ValidEpicPOIs = GetValidPOIsForBiome(Biome, ETileRarity::Epic);
                UE_LOG(LogTemp, Log, TEXT("  Valid Epic POIs: %d"), ValidEpicPOIs.Num());
                
                if (ValidEpicPOIs.Num() > 0)
                {
                    Index = CoordRandom.RandRange(0, ValidEpicPOIs.Num() - 1);
                    UE_LOG(LogTemp, Log, TEXT("  Selected epic POI index: %d / %d"), Index, ValidEpicPOIs.Num() - 1);
                    
                    EPOIType EpicPOI = ValidEpicPOIs[Index];
                    UE_LOG(LogTemp, Log, TEXT("  Upgraded to epic POI: %s"), *UEnum::GetDisplayValueAsText(EpicPOI).ToString());
                    return EpicPOI;
                }
            }

            return SelectedPOI;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("  No POI generated"));
    return EPOIType::None;
}

ETileRarity AGW_HexMapGenerator::DetermineTileRarity(EBiomeType Biome, EPOIType POI) const
{
    // Check if it's a rare biome
    bool bIsRareBiome = (Biome == EBiomeType::MysticForest || 
                         Biome == EBiomeType::PoisonSwamp || 
                         Biome == EBiomeType::DragonBones || 
                         Biome == EBiomeType::Lavascape || 
                         Biome == EBiomeType::IceSpikes);

    // Determine rarity based on POI
    if (POI == EPOIType::None)
    {
        return bIsRareBiome ? ETileRarity::Rare : ETileRarity::Basic;
    }

    // Epic POIs
    TArray<EPOIType> EpicPOIs = {
        EPOIType::Monolith, EPOIType::Oasis, EPOIType::WizardTower,
        EPOIType::Crypt, EPOIType::Rift, EPOIType::Forge,
        EPOIType::DragonPerch, EPOIType::CrystalFields
    };

    if (EpicPOIs.Contains(POI))
    {
        return ETileRarity::Epic;
    }

    // Everything else is rare
    return ETileRarity::Rare;
}

EPOIType AGW_HexMapGenerator::TrySpawnMegagon(const FGW_HexCoordinate& Coordinate, EBiomeType Biome)
{
    FGW_SeededRandom CoordRandom = RandomGenerator.FromCoordinate(Coordinate.Q, Coordinate.R);

    // Check megagon spawn chance
    if (!CoordRandom.RandBool(GenerationConfig.MegagonChance))
    {
        return EPOIType::None;
    }

    // Check if a megagon can spawn here
    if (!CanSpawnMegagon(Coordinate, Biome))
    {
        return EPOIType::None;
    }

    // Determine which megagon based on biome
    TArray<EPOIType> PossibleMegagons;

    switch (Biome)
    {
        case EBiomeType::Mountains:
            PossibleMegagons.Add(EPOIType::DragonsDen);
            PossibleMegagons.Add(EPOIType::DarkCastle);
            PossibleMegagons.Add(EPOIType::MinesOfMortem);
            break;

        case EBiomeType::Desert:
            PossibleMegagons.Add(EPOIType::AncientDragon);
            break;

        case EBiomeType::MysticForest:
        {
            PossibleMegagons.Add(EPOIType::MageCastle);
            
            // Check for dimensional stronghold requirements
            int32 RiftCount = CountNearbyPOIs(Coordinate, EPOIType::Rift, 5);
            if (RiftCount >= 3)
            {
                PossibleMegagons.Add(EPOIType::DimensionalStronghold);
            }
            break;
        }

        case EBiomeType::Lavascape:
        {
            PossibleMegagons.Add(EPOIType::Pyrolith);
            
            // Check for Great Forge requirements
            int32 ForgeCount = CountNearbyPOIs(Coordinate, EPOIType::Forge, 5);
            if (ForgeCount >= 2)
            {
                PossibleMegagons.Add(EPOIType::TheGreatForge);
            }
            break;
        }

        case EBiomeType::IceSpikes:
            PossibleMegagons.Add(EPOIType::Cryolith);
            break;

        case EBiomeType::Forest:
            PossibleMegagons.Add(EPOIType::ForestsOfGrim);
            break;

        case EBiomeType::PoisonSwamp:
            PossibleMegagons.Add(EPOIType::GabagolGrudge);
            break;

        case EBiomeType::DragonBones:
            PossibleMegagons.Add(EPOIType::CorpseOfNorridia);
            break;

        default:
            break;
    }

    if (PossibleMegagons.Num() > 0)
    {
        int32 Index = CoordRandom.RandRange(0, PossibleMegagons.Num() - 1);
        return PossibleMegagons[Index];
    }

    return EPOIType::None;
}

bool AGW_HexMapGenerator::CanSpawnMegagon(const FGW_HexCoordinate& Coordinate, EBiomeType Biome)
{
    // Check if any nearby megagons exist (prevent overlap)
    for (const FGW_HexCoordinate& ExistingMegagon : MegagonCenters)
    {
        if (ExistingMegagon.Distance(Coordinate) < 5) // Minimum distance between megagons
        {
            return false;
        }
    }

    return true;
}

int32 AGW_HexMapGenerator::CountNearbyPOIs(const FGW_HexCoordinate& Coordinate, EPOIType POIType, int32 Range) const
{
    int32 Count = 0;
    TArray<FGW_HexCoordinate> NearbyCoords = Coordinate.GetHexesInRange(Range);

    for (const FGW_HexCoordinate& NearbyCoord : NearbyCoords)
    {
        if (const FGW_HexTileData* Tile = GeneratedTiles.Find(NearbyCoord))
        {
            if (Tile->POIType == POIType)
            {
                Count++;
            }
        }
    }

    return Count;
}

TArray<EPOIType> AGW_HexMapGenerator::GetValidPOIsForBiome(EBiomeType Biome, ETileRarity Rarity) const
{
    TArray<EPOIType> ValidPOIs;

    if (Rarity == ETileRarity::Rare)
    {
        switch (Biome)
        {
            case EBiomeType::Swamp:
            case EBiomeType::PoisonSwamp:
                ValidPOIs.Add(EPOIType::Monster);
                ValidPOIs.Add(EPOIType::Ruins);
                ValidPOIs.Add(EPOIType::Tomb);
                break;

            case EBiomeType::Hills:
                ValidPOIs.Add(EPOIType::MarauderCamp);
                ValidPOIs.Add(EPOIType::Village);
                break;

            case EBiomeType::Desert:
            case EBiomeType::DragonBones:
                ValidPOIs.Add(EPOIType::Ruins);
                ValidPOIs.Add(EPOIType::Tomb);
                break;

            case EBiomeType::Forest:
            case EBiomeType::MysticForest:
                ValidPOIs.Add(EPOIType::Ruins);
                ValidPOIs.Add(EPOIType::Village);
                ValidPOIs.Add(EPOIType::Tomb);
                break;

            default:
                break;
        }
    }
    else if (Rarity == ETileRarity::Epic)
    {
        switch (Biome)
        {
            case EBiomeType::Lavascape:
                ValidPOIs.Add(EPOIType::Monolith);
                ValidPOIs.Add(EPOIType::Forge);
                break;

            case EBiomeType::IceSpikes:
                ValidPOIs.Add(EPOIType::Monolith);
                break;

            case EBiomeType::Desert:
            case EBiomeType::DragonBones:
                ValidPOIs.Add(EPOIType::Oasis);
                break;

            case EBiomeType::Mountains:
                ValidPOIs.Add(EPOIType::WizardTower);
                ValidPOIs.Add(EPOIType::Crypt);
                ValidPOIs.Add(EPOIType::DragonPerch);
                break;

            case EBiomeType::Hills:
                ValidPOIs.Add(EPOIType::Crypt);
                break;

            case EBiomeType::MysticForest:
                ValidPOIs.Add(EPOIType::Rift);
                ValidPOIs.Add(EPOIType::CrystalFields);
                break;

            default:
                break;
        }

        // Dragon Perch can also spawn next to Dragon Bones
        // This would need special handling based on neighbor checking
    }

    return ValidPOIs;
}

bool AGW_HexMapGenerator::CanPOISpawnOnBiome(EPOIType POI, EBiomeType Biome) const
{
    TArray<EPOIType> ValidPOIs = GetValidPOIsForBiome(Biome, ETileRarity::Rare);
    ValidPOIs.Append(GetValidPOIsForBiome(Biome, ETileRarity::Epic));
    return ValidPOIs.Contains(POI);
}

float AGW_HexMapGenerator::GetBiomeClusteringWeight(const FGW_HexCoordinate& Coordinate, EBiomeType TestBiome)
{
    int32 MatchingNeighbors = 0;
    int32 TotalNeighbors = 0;

    TArray<FGW_HexCoordinate> NearbyCoords = Coordinate.GetHexesInRange(GenerationConfig.BiomeClusterRadius);

    for (const FGW_HexCoordinate& NearbyCoord : NearbyCoords)
    {
        if (NearbyCoord == Coordinate) continue;

        const FGW_HexTileData* Tile = GeneratedTiles.Find(NearbyCoord);
        
        // SAFETY CHECK - only process if tile exists and is valid
        if (!Tile)
        {
            continue;  // Skip tiles that don't exist yet
        }
        
        // ADDITIONAL SAFETY - check if biome is set
        if (Tile->BiomeType == EBiomeType::None)
        {
            continue;  // Skip tiles that haven't been fully generated yet
        }

        TotalNeighbors++;
        
        // Check base biome (ignore rare variants for clustering)
        EBiomeType BaseBiome = Tile->BiomeType;
        
        // Convert rare biomes to their base for clustering comparison
        if (BaseBiome == EBiomeType::MysticForest) BaseBiome = EBiomeType::Forest;
        if (BaseBiome == EBiomeType::PoisonSwamp) BaseBiome = EBiomeType::Swamp;
        if (BaseBiome == EBiomeType::DragonBones) BaseBiome = EBiomeType::Desert;
        if (BaseBiome == EBiomeType::Lavascape || BaseBiome == EBiomeType::IceSpikes)
            BaseBiome = EBiomeType::Mountains;

        if (BaseBiome == TestBiome)
        {
            MatchingNeighbors++;
        }
    }

    if (TotalNeighbors == 0) return 0.0f;
    
    return static_cast<float>(MatchingNeighbors) / static_cast<float>(TotalNeighbors);
}

int32 AGW_HexMapGenerator::GenerateVisualVariant(const FGW_HexCoordinate& Coordinate, EBiomeType Biome)
{
    FGW_SeededRandom CoordRandom = RandomGenerator.FromCoordinate(Coordinate.Q, Coordinate.R);
    
    // Generate 0-3 for 4 visual variants (adjust as needed)
    return CoordRandom.RandRange(0, 3);
}

void AGW_HexMapGenerator::InitializePOISpawnRules()
{
    // This is a placeholder - you can expand this to have more sophisticated spawn rules
    // For now, the rules are hardcoded in the GetValidPOIsForBiome method
    
    UE_LOG(LogTemp, Log, TEXT("POI Spawn Rules Initialized"));
}

void AGW_HexMapGenerator::ForceUnlockTile(const FGW_HexCoordinate& Coordinate)
{
    // Get or generate the tile
    GetTileData(Coordinate);
    
    // Unlock it
    if (FGW_HexTileData* Tile = GeneratedTiles.Find(Coordinate))
    {
        Tile->bIsUnlocked = true;
        Tile->bIsVisible = true;
    }
}

void AGW_HexMapGenerator::ForceUnlockArea(const FGW_HexCoordinate& Center, int32 Radius)
{
    TArray<FGW_HexCoordinate> AllCoords = Center.GetHexesInRange(Radius);
    
    for (const FGW_HexCoordinate& Coord : AllCoords)
    {
        ForceUnlockTile(Coord);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Force unlocked %d tiles in radius %d around %s"), 
        AllCoords.Num(), Radius, *Center.ToString());
}