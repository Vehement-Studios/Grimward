// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
/*-------------------------------------------------------------------------*/



/**
 * Simple seeded random number generator for deterministic map generation
 * Uses Linear Congruential Generator (LCG) algorithm
 */
class FGW_SeededRandom
{
private:
    int32 Seed;      // The seed can be int32 for Blueprint
    uint32 State;    // But the state should be uint32 for proper LCG math

    // LCG constants (from Numerical Recipes)
    static constexpr uint32 A = 1664525;
    static constexpr uint32 C = 1013904223;

public:
    FGW_SeededRandom(int32 InSeed = 0)
        : Seed(InSeed)
        , State(static_cast<uint32>(InSeed))  // Convert to uint32
    {}

    // Reset to initial seed
    void Reset()
    {
        State = static_cast<uint32>(Seed);
    }

    // Set new seed
    void SetSeed(int32 InSeed)
    {
        Seed = InSeed;
        State = static_cast<uint32>(InSeed);
    }

    int32 GetSeed() const { return Seed; }

    // Get next random uint32 (internal - more reliable)
    uint32 NextUInt32()
    {
        State = (A * State + C);  // Natural uint32 overflow
        return State;
    }

    // Get random int in range [Min, Max] (inclusive)
    int32 RandRange(int32 Min, int32 Max)
    {
        if (Min > Max)
        {
            Swap(Min, Max);
        }
    
        if (Min == Max)
        {
            return Min;
        }
    
        uint32 Range = static_cast<uint32>(Max - Min + 1);
        uint32 RandomValue = NextUInt32() % Range;
    
        return Min + static_cast<int32>(RandomValue);
    }

    // Get random float in range [0, 1)
    float RandFloat()
    {
        return static_cast<float>(NextUInt32()) / 4294967296.0f;
    }

    // Get random float in range [Min, Max)
    float RandFloatRange(float Min, float Max)
    {
        return Min + (Max - Min) * RandFloat();
    }

    // Random bool with given probability (0.0 to 1.0)
    bool RandBool(float Probability = 0.5f)
    {
        return RandFloat() < Probability;
    }

    // Get a deterministic random value for a specific coordinate
    uint32 GetCoordinateSeed(int32 Q, int32 R) const
    {
        uint32 Hash = static_cast<uint32>(Seed);
        Hash = Hash * 2654435761U + static_cast<uint32>(Q);
        Hash = Hash * 2654435761U + static_cast<uint32>(R);
        return Hash;
    }

    // Create a new generator seeded from a coordinate
    FGW_SeededRandom FromCoordinate(int32 Q, int32 R) const
    {
        return FGW_SeededRandom(static_cast<int32>(GetCoordinateSeed(Q, R)));
    }
};