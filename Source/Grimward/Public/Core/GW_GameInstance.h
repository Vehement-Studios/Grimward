// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GW_GameData.h"
#include "GW_GameInstance.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Game Instance                                                          */
/*-------------------------------------------------------------------------*/
#pragma region GW_GameInstace.h
UCLASS()
class GRIMWARD_API UGW_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UGW_GameInstance();

    virtual void Init() override;

    UPROPERTY(BlueprintReadWrite, Category = "Grimward|Player Data")
    FGW_PlayerProfile PlayerProfile;

    // Data Table references
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|Data")
    UDataTable* ItemDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|Data")
    UDataTable* LevelDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grimward|Data")
    UDataTable* AchievementDataTable;

    // Player progression functions
    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    void AddExperience(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    void AddCurrency(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    bool SpendCurrency(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    void UnlockLevel(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    bool IsLevelUnlocked(FName LevelID) const;

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    void AddItemToInventory(FName ItemID);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    void RemoveItemFromInventory(FName ItemID);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    void EquipItem(FName ItemID);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    void UnequipItem(FName ItemID);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    void UnlockAchievement(FName AchievementID);

    UFUNCTION(BlueprintCallable, Category = "Grimward|Player Data")
    bool IsAchievementUnlocked(FName AchievementID) const;

    // Data retrieval functions
    UFUNCTION(BlueprintCallable, Category = "Grimward|Data")
    FGW_ItemData GetItemData(FName ItemID) const;

    UFUNCTION(BlueprintCallable, Category = "Grimward|Data")
    FGW_LevelData GetLevelData(FName LevelID) const;

    UFUNCTION(BlueprintCallable, Category = "Grimward|Data")
    FGW_AchievementData GetAchievementData(FName AchievementID) const;

    // Save/Load
    UFUNCTION(BlueprintCallable, Category = "Grimward|Save")
    void SaveGame();

    UFUNCTION(BlueprintCallable, Category = "Grimward|Save")
    void LoadGame();

    UFUNCTION(BlueprintCallable, Category = "Grimward|Save")
    void ResetProgress();

private:
    int32 CalculateExperienceForNextLevel() const;
    
    UPROPERTY()
    class UGW_SaveGame* CurrentSaveGame;

    const FString SaveSlotName = TEXT("GrimwardSave");
};
#pragma endregion
/*-------------------------------------------------------------------------*/
