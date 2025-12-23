// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_MapGenerator.generated.h"
/*-------------------------------------------------------------------------*/


UENUM(BlueprintType)
enum class EGW_HexBiome : uint8
{
	
};

UENUM(BlueprintType)
enum class EGW_HexPOI : uint8
{
	
};




UCLASS()
class GRIMWARD_API AGW_MapGenerator : public AActor
{
	GENERATED_BODY()

public:
	

private:

	static float PerlinNoise(float x, float y, float z);

	
	//void SetTile()
	
};