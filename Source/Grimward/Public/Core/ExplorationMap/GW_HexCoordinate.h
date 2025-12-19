// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GW_HexCoordinate.generated.h"
/*-------------------------------------------------------------------------*/



/**
 * Axial coordinate system for hexagonal grids
 * Uses Q (column) and R (row) coordinates
 */
USTRUCT(BlueprintType)
struct FGW_HexCoordinate
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex")
    int32 Q;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex")
    int32 R;

    FGW_HexCoordinate() : Q(0), R(0) {}
    FGW_HexCoordinate(int32 InQ, int32 InR) : Q(InQ), R(InR) {}

    // Cube coordinate conversion (for distance calculations)
    int32 GetS() const { return -Q - R; }

    // Distance between two hex coordinates
    int32 Distance(const FGW_HexCoordinate& Other) const
    {
        int32 DQ = FMath::Abs(Q - Other.Q);
        int32 DR = FMath::Abs(R - Other.R);
        int32 DS = FMath::Abs(GetS() - Other.GetS());
        return FMath::Max3(DQ, DR, DS);
    }

    // Get all 6 neighbors
    TArray<FGW_HexCoordinate> GetNeighbors() const
    {
        TArray<FGW_HexCoordinate> Neighbors;
        // The 6 axial directions
        Neighbors.Add(FGW_HexCoordinate(Q + 1, R));      // East
        Neighbors.Add(FGW_HexCoordinate(Q + 1, R - 1));  // Northeast
        Neighbors.Add(FGW_HexCoordinate(Q, R - 1));      // Northwest
        Neighbors.Add(FGW_HexCoordinate(Q - 1, R));      // West
        Neighbors.Add(FGW_HexCoordinate(Q - 1, R + 1));  // Southwest
        Neighbors.Add(FGW_HexCoordinate(Q, R + 1));      // Southeast
        return Neighbors;
    }

    // Get all hexes within a certain range
    TArray<FGW_HexCoordinate> GetHexesInRange(int32 Range) const
    {
        TArray<FGW_HexCoordinate> Results;
        for (int32 dQ = -Range; dQ <= Range; ++dQ)
        {
            int32 MinR = FMath::Max(-Range, -dQ - Range);
            int32 MaxR = FMath::Min(Range, -dQ + Range);
            for (int32 dR = MinR; dR <= MaxR; ++dR)
            {
                Results.Add(FGW_HexCoordinate(Q + dQ, R + dR));
            }
        }
        return Results;
    }

    // Get the 7-hex cluster for Megagon tiles (center + 6 neighbors)
    TArray<FGW_HexCoordinate> GetMegagonCluster() const
    {
        TArray<FGW_HexCoordinate> Cluster;
        Cluster.Add(*this); // Center
        Cluster.Append(GetNeighbors()); // 6 neighbors
        return Cluster;
    }

    // Equality operators
    bool operator==(const FGW_HexCoordinate& Other) const
    {
        return Q == Other.Q && R == Other.R;
    }

    bool operator!=(const FGW_HexCoordinate& Other) const
    {
        return !(*this == Other);
    }

    // Hash function for use in TMap
    friend int32 GetTypeHash(const FGW_HexCoordinate& Coord)
    {
        return HashCombine(GetTypeHash(Coord.Q), GetTypeHash(Coord.R));
    }

    // String representation for debugging
    FString ToString() const
    {
        return FString::Printf(TEXT("(%d, %d)"), Q, R);
    }
};