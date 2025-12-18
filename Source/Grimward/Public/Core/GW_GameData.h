// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GW_GameData.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Game Data                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_GameData.h
UENUM(BlueprintType)
enum class EGW_CharacterClass : uint8
{
    Knight UMETA(DisplayName = "Knight"),
    Wizard UMETA(DisplayName = "Wizard"),
    Rogue UMETA(DisplayName = "Rogue")
};

UENUM(BlueprintType)
enum class EGW_ItemType : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Armor UMETA(DisplayName = "Armor"),
    Consumable UMETA(DisplayName = "Consumable"),
    Ability UMETA(DisplayName = "Ability")
};

UENUM(BlueprintType)
enum class EGW_ItemRarity : uint8
{
    Common UMETA(DisplayName = "Common"),
    Uncommon UMETA(DisplayName = "Uncommon"),
    Rare UMETA(DisplayName = "Rare"),
    Epic UMETA(DisplayName = "Epic"),
    Legendary UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FGW_ItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGW_ItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGW_ItemRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Cost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StatBonus;
};

USTRUCT(BlueprintType)
struct FGW_LevelData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName LevelID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText LevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MapName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RecommendedLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BaseExperienceReward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BaseCurrencyReward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> PossibleLootItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName UnlocksLevel;
};

USTRUCT(BlueprintType)
struct FGW_AchievementData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AchievementID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText AchievementName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RewardCurrency;
};

USTRUCT(BlueprintType)
struct FGW_PlayerProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGW_CharacterClass CharacterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Experience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Currency = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> UnlockedLevels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> Inventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> EquippedItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> UnlockedAchievements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, int32> LevelCompletionCount;
};
#pragma endregion 
/*-------------------------------------------------------------------------*/