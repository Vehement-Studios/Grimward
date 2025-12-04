#pragma once

#include "CoreMinimal.h"

#include "Grimward_InventoryData.generated.h"

UENUM(BlueprintType)
enum class EGrimward_ItemType : uint8
{
    Consumable     UMETA(DisplayName = "Consumable"),
    Apparel        UMETA(DisplayName = "Apparel"),
    Weapon         UMETA(DisplayName = "Weapon"),
    Currency       UMETA(DisplayName = "Currency"),
    Miscellaneous  UMETA(DisplayName = "Miscellaneous")
}

USTRUCT(BlueprintType)
struct GRIMWARD_API FGrimward_InventoryItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory Item")
    FString ItemName = FTEXT("Default Item");

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory Item")
    TObjectPtr<UTexture2D> ItemIcon = nullptr;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory Item")
    EGrimward_ItemType ItemType = EGrimward_ItemType::Miscellaneous;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory Item")
    int32 Quantity = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory Item")
    int32 Quality = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory Item")
    bool bIsStackable = true;
};