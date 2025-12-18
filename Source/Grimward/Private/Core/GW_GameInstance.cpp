// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Public/Core/GW_GameInstance.h"
#include "Public/Core/GW_SaveGame.h"
#include "Kismet/GameplayStatics.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_GameInstace.cpp
UGW_GameInstance::UGW_GameInstance()
{
    // Initialize with default unlocked level
    PlayerProfile.UnlockedLevels.Add(FName("Level_01"));
}

void UGW_GameInstance::Init()
{
    Super::Init();
    
    // Try to load existing save
    LoadGame();
}

void UGW_GameInstance::AddExperience(int32 Amount)
{
    PlayerProfile.Experience += Amount;
    
    // Check for level up
    int32 ExpNeeded = CalculateExperienceForNextLevel();
    while (PlayerProfile.Experience >= ExpNeeded)
    {
        PlayerProfile.Experience -= ExpNeeded;
        PlayerProfile.Level++;
        ExpNeeded = CalculateExperienceForNextLevel();
        
        UE_LOG(LogTemp, Log, TEXT("[Grimward] Level Up! New Level: %d"), PlayerProfile.Level);
        
        // You can broadcast a delegate here for UI updates
    }
    
    SaveGame();
}

void UGW_GameInstance::AddCurrency(int32 Amount)
{
    PlayerProfile.Currency += Amount;
    SaveGame();
}

bool UGW_GameInstance::SpendCurrency(int32 Amount)
{
    if (PlayerProfile.Currency >= Amount)
    {
        PlayerProfile.Currency -= Amount;
        SaveGame();
        return true;
    }
    return false;
}

void UGW_GameInstance::UnlockLevel(FName LevelID)
{
    if (!PlayerProfile.UnlockedLevels.Contains(LevelID))
    {
        PlayerProfile.UnlockedLevels.Add(LevelID);
        UE_LOG(LogTemp, Log, TEXT("[Grimward] Unlocked Level: %s"), *LevelID.ToString());
        SaveGame();
    }
}

bool UGW_GameInstance::IsLevelUnlocked(FName LevelID) const
{
    return PlayerProfile.UnlockedLevels.Contains(LevelID);
}

void UGW_GameInstance::AddItemToInventory(FName ItemID)
{
    PlayerProfile.Inventory.Add(ItemID);
    UE_LOG(LogTemp, Log, TEXT("[Grimward] Added item to inventory: %s"), *ItemID.ToString());
    SaveGame();
}

void UGW_GameInstance::RemoveItemFromInventory(FName ItemID)
{
    PlayerProfile.Inventory.Remove(ItemID);
    SaveGame();
}

void UGW_GameInstance::EquipItem(FName ItemID)
{
    if (PlayerProfile.Inventory.Contains(ItemID))
    {
        PlayerProfile.EquippedItems.AddUnique(ItemID);
        UE_LOG(LogTemp, Log, TEXT("[Grimward] Equipped item: %s"), *ItemID.ToString());
        SaveGame();
    }
}

void UGW_GameInstance::UnequipItem(FName ItemID)
{
    PlayerProfile.EquippedItems.Remove(ItemID);
    SaveGame();
}

void UGW_GameInstance::UnlockAchievement(FName AchievementID)
{
    if (!PlayerProfile.UnlockedAchievements.Contains(AchievementID))
    {
        PlayerProfile.UnlockedAchievements.Add(AchievementID);
        
        // Award achievement rewards
        FGW_AchievementData AchData = GetAchievementData(AchievementID);
        if (AchData.RewardCurrency > 0)
        {
            AddCurrency(AchData.RewardCurrency);
        }
        
        UE_LOG(LogTemp, Log, TEXT("[Grimward] Achievement Unlocked: %s"), *AchievementID.ToString());
        SaveGame();
    }
}

bool UGW_GameInstance::IsAchievementUnlocked(FName AchievementID) const
{
    return PlayerProfile.UnlockedAchievements.Contains(AchievementID);
}

FGW_ItemData UGW_GameInstance::GetItemData(FName ItemID) const
{
    if (ItemDataTable)
    {
        FGW_ItemData* ItemData = ItemDataTable->FindRow<FGW_ItemData>(ItemID, TEXT(""));
        if (ItemData)
        {
            return *ItemData;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("[Grimward] Could not find item data for: %s"), *ItemID.ToString());
    return FGW_ItemData();
}

FGW_LevelData UGW_GameInstance::GetLevelData(FName LevelID) const
{
    if (LevelDataTable)
    {
        FGW_LevelData* LevelData = LevelDataTable->FindRow<FGW_LevelData>(LevelID, TEXT(""));
        if (LevelData)
        {
            return *LevelData;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("[Grimward] Could not find level data for: %s"), *LevelID.ToString());
    return FGW_LevelData();
}

FGW_AchievementData UGW_GameInstance::GetAchievementData(FName AchievementID) const
{
    if (AchievementDataTable)
    {
        FGW_AchievementData* AchData = AchievementDataTable->FindRow<FGW_AchievementData>(AchievementID, TEXT(""));
        if (AchData)
        {
            return *AchData;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("[Grimward] Could not find achievement data for: %s"), *AchievementID.ToString());
    return FGW_AchievementData();
}

int32 UGW_GameInstance::CalculateExperienceForNextLevel() const
{
    // Exponential curve: 100 * Level^1.5
    return FMath::RoundToInt(100.0f * FMath::Pow(static_cast<float>(PlayerProfile.Level), 1.5f));
}

void UGW_GameInstance::SaveGame()
{
    UGW_SaveGame* SaveGameInstance = Cast<UGW_SaveGame>(
        UGameplayStatics::CreateSaveGameObject(UGW_SaveGame::StaticClass())
    );
    
    if (SaveGameInstance)
    {
        SaveGameInstance->PlayerProfile = PlayerProfile;
        
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0))
        {
            UE_LOG(LogTemp, Log, TEXT("[Grimward] Game saved successfully"));
        }
    }
}

void UGW_GameInstance::LoadGame()
{
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        UGW_SaveGame* LoadedGame = Cast<UGW_SaveGame>(
            UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)
        );
        
        if (LoadedGame)
        {
            PlayerProfile = LoadedGame->PlayerProfile;
            UE_LOG(LogTemp, Log, TEXT("[Grimward] Game loaded successfully"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[Grimward] No save file found, starting new game"));
    }
}

void UGW_GameInstance::ResetProgress()
{
    PlayerProfile = FGW_PlayerProfile();
    PlayerProfile.UnlockedLevels.Add(FName("Level_01"));
    SaveGame();
    UE_LOG(LogTemp, Log, TEXT("[Grimward] Progress reset"));
}
#pragma endregion
/*-------------------------------------------------------------------------*/