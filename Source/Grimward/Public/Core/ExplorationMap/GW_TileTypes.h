// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "GW_TileTypes.generated.h"
/*-------------------------------------------------------------------------*/


UENUM(BlueprintType)
enum class EGW_GenerationPresets : uint8
{
	LargeBiomes,
	MediumBiomes,
	SmallBiomes
};

UENUM(BlueprintType)
enum class EGW_HexBiome : uint8
{
	Hill,
	Forest,
	Mountain,
	Desert,
	Swamp,
	MysticForest,
	PoisonousSwamp,
	DragonBoneyard,
	Lavascape,
	IceSpike,
	GreatPeak,
	Water
};

UENUM(BlueprintType)
enum class EGW_HexPOI : uint8
{
	None,
	Monster,
	MarauderCamp,
	Ruin,
	Labyrinth,
	Village,
	Tomb,
	Monolith,
	Oasis,
	WizardTower,
	Crypt,
	Temple,
	Rift,
	Forge,
	DragonPerch,
	CrystalField,
};

UENUM(BlueprintType)
enum class EGW_Megagon : uint8
{
	DragonDen,
	MinotaurLabrinth,
	AncientDragon,
	MageCastle,
	DarkCastle,
	Pyrolith,
	Cryolith,
	ForestsOfGrim,
	GabagolGrudge,
	CorpseOfNorridia,
	MinesOfMortem,
	TheGreatForge,
	DimensionalStronghold
};