// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Core/ExplorationMap/GW_HexMapGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ExplorationMap/GW_HexMapSaveData.h"
/*-------------------------------------------------------------------------*/




bool AGW_HexMapGenerator::SaveMapState(const FString& SlotName)
{
    UGW_HexMapSaveData* SaveData = Cast<UGW_HexMapSaveData>(
        UGameplayStatics::CreateSaveGameObject(UGW_HexMapSaveData::StaticClass()));

    if (!SaveData)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
        return false;
    }

    // Save basic map info
    SaveData->MapSeed = MapSeed;
    SaveData->ViewRange = CurrentViewRange;
    SaveData->SaveSlotName = SlotName;
    SaveData->LastSaveTime = FDateTime::Now();

    // Save conquered tiles
    SaveData->ConqueredTiles.Empty();
    for (const auto& Pair : GeneratedTiles)
    {
        if (Pair.Value.bIsConquered)
        {
            FTileSaveData TileSave;
            TileSave.Coordinate = Pair.Key;
            TileSave.bIsConquered = true;
            SaveData->ConqueredTiles.Add(TileSave);
        }
    }

    // Calculate stats
    SaveData->TotalTilesExplored = GetConqueredTileCount();
    
    int32 MegagonCount = 0;
    for (const auto& Pair : GeneratedTiles)
    {
        if (Pair.Value.bIsConquered && Pair.Value.Rarity == ETileRarity::Megagon)
        {
            MegagonCount++;
        }
    }
    SaveData->TotalMegagonsDefeated = MegagonCount;

    // Save to disk
    bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveData, SlotName, 0);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Map saved successfully to slot: %s"), *SlotName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save map to slot: %s"), *SlotName);
    }

    return bSuccess;
}

bool AGW_HexMapGenerator::LoadMapState(const FString& SlotName)
{
    if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("Save game does not exist: %s"), *SlotName);
        return false;
    }

    UGW_HexMapSaveData* SaveData = Cast<UGW_HexMapSaveData>(
        UGameplayStatics::LoadGameFromSlot(SlotName, 0));

    if (!SaveData)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load save game: %s"), *SlotName);
        return false;
    }

    // Clear current state
    GeneratedTiles.Empty();
    MegagonCenters.Empty();

    // Restore seed
    MapSeed = SaveData->MapSeed;
    CurrentViewRange = SaveData->ViewRange;
    RandomGenerator.SetSeed(MapSeed);

    // Regenerate and mark conquered tiles
    for (const FTileSaveData& TileSave : SaveData->ConqueredTiles)
    {
        // Generate the tile (and surrounding tiles)
        FGW_HexTileData TileData = GetTileData(TileSave.Coordinate);
        
        // Mark as conquered
        if (FGW_HexTileData* Tile = GeneratedTiles.Find(TileSave.Coordinate))
        {
            Tile->bIsConquered = true;
            Tile->bIsUnlocked = true;
            
            // Unlock adjacent tiles
            UnlockAdjacentTiles(TileSave.Coordinate);
        }
    }

    // Update visibility for all conquered tiles
    UpdateVisibility(CurrentViewRange);

    UE_LOG(LogTemp, Log, TEXT("Map loaded successfully from slot: %s"), *SlotName);
    UE_LOG(LogTemp, Log, TEXT("Seed: %u, Conquered Tiles: %d"), 
        MapSeed, SaveData->ConqueredTiles.Num());

    return true;
}

bool AGW_HexMapGenerator::DoesSaveExist(const FString& SlotName)
{
    return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

bool AGW_HexMapGenerator::DeleteSave(const FString& SlotName)
{
    return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}