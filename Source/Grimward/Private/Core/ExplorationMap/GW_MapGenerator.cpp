// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Core/ExplorationMap/GW_MapGenerator.h"
#include "Engine/Texture2D.h"
#include "Kismet/KismetMathLibrary.h"
/*-------------------------------------------------------------------------*/

AGW_MapGenerator::AGW_MapGenerator()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGW_MapGenerator::BeginPlay()
{
    Super::BeginPlay();
    InitializeBiomeData();
}

void AGW_MapGenerator::InitializeBiomeData()
{
    // TODO: This is where we could make a preset generator for generating different types of worlds.
    
    // Hills - Common grasslands and rolling terrain
    FGW_BiomeGenerationInfo HillsData;
    HillsData.HexBiomeWeights.Add(EGW_HexBiome::Hill, 1.0f);
    BiomeDataConfig.Add(TEXT("hills"), HillsData);
    
    // Forest - Temperate woodlands
    FGW_BiomeGenerationInfo ForestData;
    ForestData.HexBiomeWeights.Add(EGW_HexBiome::Forest, 0.9f);
    ForestData.HexBiomeWeights.Add(EGW_HexBiome::Hill, 0.1f);
    BiomeDataConfig.Add(TEXT("forest"), ForestData);
    
    // Mountain - High altitude rocky terrain
    FGW_BiomeGenerationInfo MountainData;
    MountainData.HexBiomeWeights.Add(EGW_HexBiome::Mountain, 0.95f);
    MountainData.HexBiomeWeights.Add(EGW_HexBiome::Hill, 0.05f);
    BiomeDataConfig.Add(TEXT("mountain"), MountainData);
    
    // Great Peak - Extreme mountain peaks (can only spawn in mountain regions)
    FGW_BiomeGenerationInfo GreatPeakData;
    GreatPeakData.HexBiomeWeights.Add(EGW_HexBiome::GreatPeak, 0.9f);
    GreatPeakData.HexBiomeWeights.Add(EGW_HexBiome::Mountain, 0.1f);
    BiomeDataConfig.Add(TEXT("great_peak"), GreatPeakData);
    
    // Desert - Hot, dry wastelands
    FGW_BiomeGenerationInfo DesertData;
    DesertData.HexBiomeWeights.Add(EGW_HexBiome::Desert, 1.0f);
    BiomeDataConfig.Add(TEXT("desert"), DesertData);
    
    // Swamp - Wet lowlands
    FGW_BiomeGenerationInfo SwampData;
    SwampData.HexBiomeWeights.Add(EGW_HexBiome::Swamp, 1.0f);
    BiomeDataConfig.Add(TEXT("swamp"), SwampData);
    
    // Ice Spike - Extreme cold high altitude
    FGW_BiomeGenerationInfo IceSpikeData;
    IceSpikeData.HexBiomeWeights.Add(EGW_HexBiome::IceSpike, 0.9f);
    IceSpikeData.HexBiomeWeights.Add(EGW_HexBiome::Mountain, 0.1f);
    BiomeDataConfig.Add(TEXT("ice_spike"), IceSpikeData);
    
    // Water - Lakes, rivers, oceans
    FGW_BiomeGenerationInfo WaterData;
    WaterData.HexBiomeWeights.Add(EGW_HexBiome::Water, 1.0f);
    BiomeDataConfig.Add(TEXT("water"), WaterData);
    
    // Mystic Forest - Enchanted woodlands (requires high enchantment)
    FGW_BiomeGenerationInfo MysticForestData;
    MysticForestData.HexBiomeWeights.Add(EGW_HexBiome::MysticForest, 0.95f);
    MysticForestData.HexBiomeWeights.Add(EGW_HexBiome::Forest, 0.05f);
    BiomeDataConfig.Add(TEXT("mystic_forest"), MysticForestData);
    
    // Poisonous Swamp - Toxic wetlands (requires high enchantment)
    FGW_BiomeGenerationInfo PoisonousSwampData;
    PoisonousSwampData.HexBiomeWeights.Add(EGW_HexBiome::PoisonousSwamp, 0.95f);
    PoisonousSwampData.HexBiomeWeights.Add(EGW_HexBiome::Swamp, 0.05f);
    BiomeDataConfig.Add(TEXT("poisonous_swamp"), PoisonousSwampData);
    
    // Dragon Boneyard - Ancient dragon graveyard (requires high enchantment)
    FGW_BiomeGenerationInfo DragonBoneyardData;
    DragonBoneyardData.HexBiomeWeights.Add(EGW_HexBiome::DragonBoneyard, 1.0f);
    BiomeDataConfig.Add(TEXT("dragon_boneyard"), DragonBoneyardData);
    
    // Lavascape - Volcanic hellscape (requires high temperature + enchantment)
    FGW_BiomeGenerationInfo LavascapeData;
    LavascapeData.HexBiomeWeights.Add(EGW_HexBiome::Lavascape, 1.0f);
    BiomeDataConfig.Add(TEXT("lavascape"), LavascapeData);
}

void AGW_MapGenerator::GenerateBiomeMap(int32 InSeed)
{
    // Set seed (use random if seed is 0)
    Seed = (InSeed == 0) ? FMath::Rand() : InSeed;
    RandomStream.Initialize(Seed);
    
    // Clear existing data
    TemperatureMap.Empty();
    MoistureMap.Empty();
    AltitudeMap.Empty();
    VolatilityMap.Empty();
    EnchantmentMap.Empty();
    BiomeMap.Empty();
    
    // Generate all noise maps
    TemperatureMap = GenerateNoiseMap(TemperaturePeriod, TemperatureOctaves, Seed);
    MoistureMap = GenerateNoiseMap(MoisturePeriod, MoistureOctaves, Seed + 1000);
    AltitudeMap = GenerateNoiseMap(AltitudePeriod, AltitudeOctaves, Seed + 2000);
    VolatilityMap = GenerateNoiseMap(VolatilityPeriod, VolatilityOctaves, Seed + 3000);
    EnchantmentMap = GenerateNoiseMap(EnchantmentPeriod, EnchantmentOctaves, Seed + 4000);
    
    // Determine biomes for each position
    DetermmineBiomes();
    
    UE_LOG(LogTemp, Log, TEXT("Biome map generated with seed: %d, Size: %dx%d"), Seed, GenWidth, GenHeight);
}

TMap<FIntPoint, float> AGW_MapGenerator::GenerateNoiseMap(float Period, int32 Octaves, int32 NoiseSeed)
{
    TMap<FIntPoint, float> NoiseMap;
    
    for (int32 X = 0; X < GenWidth; X++)
    {
        for (int32 Y = 0; Y < GenHeight; Y++)
        {
            float NoiseValue = GetPerlinNoise2D(X, Y, Period, Octaves, NoiseSeed);
            // Convert to 0-2 range and take absolute value
            NoiseValue = 2.0f * FMath::Abs(NoiseValue);
            NoiseMap.Add(FIntPoint(X, Y), NoiseValue);
        }
    }
    
    return NoiseMap;
}

float AGW_MapGenerator::GetPerlinNoise2D(float X, float Y, float Period, int32 Octaves, int32 NoiseSeed)
{
    float Total = 0.0f;
    float Frequency = 1.0f / Period;
    float Amplitude = 1.0f;
    float MaxValue = 0.0f;
    
    for (int32 i = 0; i < Octaves; i++)
    {
        float SampleX = X * Frequency;
        float SampleY = Y * Frequency;
        
        // Hash-based offset for seed variation
        int32 Hash = NoiseSeed + i * 1000;
        float NoiseValue = FMath::PerlinNoise2D(FVector2D(SampleX + Hash * 0.1f, SampleY + Hash * 0.1f));
        
        Total += NoiseValue * Amplitude;
        MaxValue += Amplitude;
        
        Amplitude *= 0.5f;
        Frequency *= 2.0f;
    }
    
    return Total / MaxValue;
}

void AGW_MapGenerator::DetermmineBiomes()
{
    for (int32 X = 0; X < GenWidth; X++)
    {
        for (int32 Y = 0; Y < GenHeight; Y++)
        {
            FIntPoint Pos(X, Y);
            
            float Alt = AltitudeMap[Pos];
            float Temp = TemperatureMap[Pos];
            float Moist = MoistureMap[Pos];
            float Volat = VolatilityMap[Pos];
            float Ench = EnchantmentMap[Pos];
            
            FGW_BiomeData BiomeData;
            BiomeData.Altitude = Alt;
            BiomeData.Temperature = Temp;
            BiomeData.Moisture = Moist;
            BiomeData.Volatility = Volat;
            BiomeData.Enchantment = Ench;
            
            // Determine which category this position falls into, then randomly select a biome
            FString Category;
            BiomeData.BiomeEntry = DetermineBiomeCategory(Alt, Temp, Moist, Ench, Category);
            
            BiomeMap.Add(Pos, BiomeData);
        }
    }
}

EGW_HexBiome AGW_MapGenerator::DetermineBiomeCategory(float Altitude, float Temperature, float Moisture, float Enchantment, FString& OutCategory)
{
    // WATER CHECK - Multiple conditions for water spawning
    
    // Very high moisture = water anywhere (oceans, large lakes)
    if (Moisture > 1.3f)  // Lowered from 1.5f
    {
        OutCategory = TEXT("water");
        return GetRandomTileForCategory(OutCategory);
    }
    
    // Low altitude + high moisture = coastal water/seas
    if (Altitude < 0.2f && Moisture > 1.0f)
    {
        OutCategory = TEXT("water");
        return GetRandomTileForCategory(OutCategory);
    }
    
    // EXTREME ALTITUDE - Mountains, Ice Spikes, and Great Peaks
    if (Altitude > 1.2f)
    {
        // Great Peaks - Only spawn at extreme altitude with high enchantment
        if (Altitude > 1.5f && Enchantment > 1.3f)
        {
            OutCategory = TEXT("great_peak");
            return GetRandomTileForCategory(OutCategory);
        }
        // Very cold high mountains become ice spikes
        else if (Temperature < 0.4f)
        {
            OutCategory = TEXT("ice_spike");
            return GetRandomTileForCategory(OutCategory);
        }
        // High mountains
        else
        {
            OutCategory = TEXT("mountain");
            return GetRandomTileForCategory(OutCategory);
        }
    }
    // HIGH ALTITUDE - Mountains
    else if (IsBetween(Altitude, 0.9f, 1.2f))
    {
        // Great Peaks can also spawn in high mountains with very high enchantment
        if (Enchantment > 1.5f && RandomStream.FRand() < 0.3f)
        {
            OutCategory = TEXT("great_peak");
            return GetRandomTileForCategory(OutCategory);
        }
        
        OutCategory = TEXT("mountain");
        return GetRandomTileForCategory(OutCategory);
    }
    // MODERATE ALTITUDE - Most biomes
    else if (IsBetween(Altitude, 0.3f, 0.9f))
    {
        // Add water spawning in moderate altitude too (lakes)
        if (Moisture > 1.1f)
        {
            OutCategory = TEXT("water");
            return GetRandomTileForCategory(OutCategory);
        }
        
        // COLD REGIONS
        if (Temperature < 0.5f)
        {
            // Wet and cold = Forest
            if (Moisture > 0.7f)
            {
                OutCategory = TEXT("forest");
                return GetRandomTileForCategory(OutCategory);
            }
            // Moderate moisture = Hills
            else
            {
                OutCategory = TEXT("hills");
                return GetRandomTileForCategory(OutCategory);
            }
        }
        // TEMPERATE REGIONS
        else if (IsBetween(Temperature, 0.5f, 1.0f))
        {
            // Very wet = Forest
            if (Moisture > 0.8f)
            {
                OutCategory = TEXT("forest");
                return GetRandomTileForCategory(OutCategory);
            }
            // Moderate wet = Hills
            else if (IsBetween(Moisture, 0.4f, 0.8f))
            {
                OutCategory = TEXT("hills");
                return GetRandomTileForCategory(OutCategory);
            }
            // Dry = Desert transition
            else
            {
                OutCategory = TEXT("desert");
                return GetRandomTileForCategory(OutCategory);
            }
        }
        // HOT REGIONS
        else // Temperature > 1.0f
        {
            // Hot and wet = could be mystic if enchanted
            if (Moisture > 0.7f)
            {
                OutCategory = TEXT("forest");
                return GetRandomTileForCategory(OutCategory);
            }
            // Hot and dry = Desert or Lavascape (if very hot and enchanted)
            else
            {
                OutCategory = TEXT("desert");
                return GetRandomTileForCategory(OutCategory);
            }
        }
    }
    // LOW ALTITUDE - Swamps, water, and lowlands
    else // Altitude < 0.3f
    {
        // Very wet lowlands = Water (oceans, lakes) - even more aggressive
        if (Moisture > 0.9f)  // Lowered from 1.2f
        {
            OutCategory = TEXT("water");
            return GetRandomTileForCategory(OutCategory);
        }
        // Moderately wet lowlands = Swamp
        else if (Moisture > 0.7f)  // Lowered from 0.8f
        {
            OutCategory = TEXT("swamp");
            return GetRandomTileForCategory(OutCategory);
        }
        // Hot and dry lowlands = Desert
        else if (Temperature > 1.0f && Moisture < 0.4f)
        {
            OutCategory = TEXT("desert");
            return GetRandomTileForCategory(OutCategory);
        }
        // Default lowland = Hills
        else
        {
            OutCategory = TEXT("hills");
            return GetRandomTileForCategory(OutCategory);
        }
    }
}

EGW_HexBiome AGW_MapGenerator::GetRandomTileForCategory(const FString& Category)
{
    if (!BiomeDataConfig.Contains(Category))
    {
        return EGW_HexBiome::Hill;
    }
    
    const FGW_BiomeGenerationInfo& BiomeInfo = BiomeDataConfig[Category];
    float RandomValue = RandomStream.FRand();
    float RunningTotal = 0.0f;
    
    for (const auto& BiomePair : BiomeInfo.HexBiomeWeights)
    {
        RunningTotal += BiomePair.Value;
        if (RandomValue <= RunningTotal)
        {
            return BiomePair.Key;
        }
    }
    
    // Fallback to first biome type in the category
    if (BiomeInfo.HexBiomeWeights.Num() > 0)
    {
        auto Iterator = BiomeInfo.HexBiomeWeights.CreateConstIterator();
        return Iterator.Key();
    }
    
    return EGW_HexBiome::Hill;
}

bool AGW_MapGenerator::IsBetween(float Value, float Start, float End) const
{
    return Value >= Start && Value < End;
}

FGW_BiomeData AGW_MapGenerator::GetBiomeDataAt(int32 X, int32 Y) const
{
    FIntPoint Pos(X, Y);
    if (BiomeMap.Contains(Pos))
    {
        return BiomeMap[Pos];
    }
    
    return FGW_BiomeData();
}

UTexture2D* AGW_MapGenerator::GenerateTestDebugTexture()
{
    if (BiomeMap.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("BiomeMap is empty. Generate biome map first."));
        return nullptr;
    }
    
    // Create a new texture
    UTexture2D* Texture = UTexture2D::CreateTransient(GenWidth, GenHeight, PF_B8G8R8A8);
    if (!Texture)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create texture."));
        return nullptr;
    }
    
    // Lock the texture for editing
    FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
    FColor* ColorData = static_cast<FColor*>(Data);
    
    // Fill texture with biome colors
    for (int32 Y = 0; Y < GenHeight; Y++)
    {
        for (int32 X = 0; X < GenWidth; X++)
        {
            FIntPoint Pos(X, Y);
            if (BiomeMap.Contains(Pos))
            {
                const FGW_BiomeData& BiomeData = BiomeMap[Pos];
                FColor Color = GetColorForBiome(BiomeData.BiomeEntry);
                ColorData[Y * GenWidth + X] = Color;
            }
            else
            {
                ColorData[Y * GenWidth + X] = FColor::Black;
            }
        }
    }
    
    // Unlock and update
    Mip.BulkData.Unlock();
    Texture->UpdateResource();
    
    UE_LOG(LogTemp, Log, TEXT("Debug texture generated successfully."));
    
    return Texture;
}

FColor AGW_MapGenerator::GetColorForBiome(EGW_HexBiome Biome) const
{
    switch (Biome)
    {
        case EGW_HexBiome::Hill:
            return FColor(144, 238, 144);           // Light Green
        case EGW_HexBiome::Forest:
            return FColor(34, 139, 34);             // Forest Green
        case EGW_HexBiome::Mountain:
            return FColor(139, 137, 137);           // Gray
        case EGW_HexBiome::Desert:
            return FColor(237, 201, 175);           // Sandy Tan
        case EGW_HexBiome::Swamp:
            return FColor(107, 142, 35);            // Olive Drab
        case EGW_HexBiome::MysticForest:
            return FColor(138, 43, 226);            // Blue Violet
        case EGW_HexBiome::PoisonousSwamp:
            return FColor(154, 205, 50);            // Yellow Green
        case EGW_HexBiome::DragonBoneyard:
            return FColor(245, 245, 220);           // Beige
        case EGW_HexBiome::Lavascape:
            return FColor(255, 69, 0);              // Orange Red
        case EGW_HexBiome::IceSpike:
            return FColor(175, 238, 238);           // Pale Turquoise
        case EGW_HexBiome::Water:
            return FColor(65, 105, 225);            // Royal Blue
        case EGW_HexBiome::GreatPeak:
            return FColor(255, 255, 255);           // White (snow-capped peaks)
        default:
            return FColor::Magenta;                 // Error color
    }
}