#pragma once

#include "CoreMinimal.h"
#include "Grimward_InventoryData.h"
#include "Grimward_PlayerData.generated.h"

UENUM(BlueprintType)
enum class EGrimward_EquipmentSlot : uint8
{
    Weapon      UMETA(DisplayName = "Weapon"),
    Head        UMETA(DisplayName = "Head"),
    Chest       UMETA(DisplayName = "Chest"),
    Legs        UMETA(DisplayName = "Legs"),
    Back        UMETA(DisplayName = "Back"),
    Shoes       UMETA(DisplayName = "Shoes"),
    Belt        UMETA(DisplayName = "Belt"),
    Necklace    UMETA(DisplayName = "Necklace")
}

USTRUCT(BlueprintType)
struct GRIMWARD_API FGrimward_PlayerEquipmentSlots
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Equipment")
    EGrimward_EquipmentSlot Weapon = EGrimward_EquipmentSlot::Weapon;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Equipment")
    EGrimward_EquipmentSlot Head = EGrimward_EquipmentSlot::Head;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Equipment")
    EGrimward_EquipmentSlot Chest = EGrimward_EquipmentSlot::Chest;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Equipment")
    EGrimward_EquipmentSlot Legs = EGrimward_EquipmentSlot::Legs;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Equipment")
    EGrimward_EquipmentSlot Back = EGrimward_EquipmentSlot::Back;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Equipment")
    EGrimward_EquipmentSlot Shoes = EGrimward_EquipmentSlot::Shoes;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Equipment")
    EGrimward_EquipmentSlot Belt = EGrimward_EquipmentSlot::Belt;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Equipment")
    EGrimward_EquipmentSlot Necklace = EGrimward_EquipmentSlot::Necklace;
}

USTRUCT(BlueprintType)
struct GRIMWARD_API FGrimward_PlayerDerivedStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Derived Stats")
    float AttackPower = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Derived Stats")
    float Defense = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Derived Stats")
    float Armor = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Derived Stats")
    float DodgeChance = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Derived Stats")
    float CritChance = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Derived Stats")
    float CritMultiplier = 0.f;
}

USTRUCT(BlueprintType)
struct GRIMWARD_API FGrimward_PlayerPrimaryStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Stats")
    int32 Strength = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Stats")
    int32 Agility = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Stats")
    int32 Spirit = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Stats")
    int32 Learning = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Stats")
    int32 Luck = 0;
}

USTRUCT(BlueprintType)
struct GRIMWARD_API FGrimward_PlayerStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Stats")
    FGrimward_PlayerPrimaryStats PrimaryStats;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Stats")
    FGrimward_PlayerDerivedStats DerivedStats;
}

USTRUCT(BlueprintType)
struct GRIMWARD_API FGrimward_PlayerData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Data")
    FString PlayerName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Data")
    int32 PlayerLevel = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Data")
    float Health = 100.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Data")
    float Stamina = 100.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Data")
    FGrimward_PlayerStats PlayerStats;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Data")
    int32 MaxInventorySize = 20;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Data")
    TArray<FString> InventoryItems;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Data")
    TMap<EGrimward_EquipmentSlot, FString> Equipment;
}
