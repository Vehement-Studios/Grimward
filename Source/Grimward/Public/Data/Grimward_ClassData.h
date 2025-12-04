#pragma once

#include "CoreMinimal.h"

#include "Grimward_PlayerData.generated.h"

UENUM(BlueprintType)
enum class EGrimward_ClassType : uint8
{
    Knight     UMETA(DisplayName = "Knight"),
    Rogue      UMETA(DisplayName = "Rogue"),
    Wizard     UMETA(DisplayName = "Wizard"),
}

USTRUCT(BlueprintType)
struct GRIMWARD_API FGrimward_ClassData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Class Data")
    EGrimward_ClassType ClassType = EGrimward_ClassType::Knight;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Class Data")
    FString ClassDescription = FTEXT("Default Class Description");
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Class Data")
    FGrimward_StartingStats StartingStats;
};