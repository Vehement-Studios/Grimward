// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GW_TileTypes.h"
#include "GameFramework/Actor.h"
#include "GW_MapGenerator.generated.h"
/*-------------------------------------------------------------------------*/


USTRUCT(BlueprintType)
struct FGW_BiomeGenerationInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGW_HexBiome, float> HexBiomeWeights;		// Stores a map of the biome weights.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGW_HexPOI, float> HexPOIWeights;			// Stores a map of the POI roll weights.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGW_Megagon, float> MegagonWeights;		// Stores a map of the Megagon roll weights.
};

USTRUCT(BlueprintType)
struct FGW_BiomeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Temperature;								// Determines the hotlands or coldlands.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Moisture;									// Determines the wetlands and drylands.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Altitude;									// Determines the highlands and lowlands.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Volatility;								// Determines the amount of POIs.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Enchantment;								// Determines the presence of rare biomes.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGW_HexBiome BiomeEntry;
};


UCLASS()
class GRIMWARD_API AGW_MapGenerator : public AActor
{
	GENERATED_BODY()

public:
	AGW_MapGenerator();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 GenWidth = 128;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 GenHeight = 128;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 Seed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	float TemperaturePeriod = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 TemperatureOctaves = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	float MoisturePeriod = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 MoistureOctaves = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	float AltitudePeriod = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 AltitudeOctaves = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	float VolatilityPeriod = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 VolatilityOctaves = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	float EnchantmentPeriod = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 EnchantmentOctaves = 1;
	
	UFUNCTION(BlueprintCallable, Category = "Generation")
	void GenerateBiomeMap(int32 InSeed);
	
	UFUNCTION(BlueprintCallable, Category = "Generation")
	FGW_BiomeData GetBiomeDataAt(int32 X, int32 Y) const;
	
	UFUNCTION(BlueprintCallable, Category = "Generation")
	UTexture2D* GenerateTestDebugTexture();
	
	UFUNCTION(BlueprintCallable, Category = "Generation")
	const TMap<FIntPoint, FGW_BiomeData>& GetBiomeMap() const { return BiomeMap; }

protected:
	virtual void BeginPlay() override;
	
private:
	// Stores the generated data here:
	TMap<FIntPoint, float> TemperatureMap;
	TMap<FIntPoint, float> MoistureMap;
	TMap<FIntPoint, float> AltitudeMap;
	TMap<FIntPoint, float> VolatilityMap;
	TMap<FIntPoint, float> EnchantmentMap;
	TMap<FIntPoint, FGW_BiomeData> BiomeMap;
	
	// Biome configuration settings - maps categories to probabilities. TODO: Implement presets?
	TMap<FString, FGW_BiomeGenerationInfo> BiomeDataConfig;
	
	// Helper functions:
	TMap<FIntPoint, float> GenerateNoiseMap(float Period, int32 Octaves, int32 NoiseSeed);
	float GetPerlinNoise2D(float X, float Y, float Period, int32 Octaves, int32 NoiseSeed);
	void DetermmineBiomes();
	EGW_HexBiome DetermineBiomeCategory(float Altitude, float Temperature, float Moisture, float Enchantment, FString& OutCategory);
	EGW_HexBiome GetRandomTileForCategory(const FString& Category);
	bool IsBetween(float Value, float Start, float End) const;	
	void InitializeBiomeData();
	FColor GetColorForBiome(EGW_HexBiome Biome) const;
	
	
	FRandomStream RandomStream;

};