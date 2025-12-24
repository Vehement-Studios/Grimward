// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/ExplorationMap/GW_TileTypes.h"
#include "GW_HexTile.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/


// This is the data associated with an individual tile. 


/*-------------------------------------------------------------------------*/
/*  Hex Tile			                                                   */
/*-------------------------------------------------------------------------*/
#pragma region GW_HexTile.h
UCLASS()
class GRIMWARD_API UGW_HexTile : public UUserWidget
{
	GENERATED_BODY()


	
private:

	
	bool bExplored;		// This determines if the tile is viewable by the player or darkened and hidden.
	bool bConquered;	// This determines if the player has conquered the tile.

	
};
#pragma endregion
/*-------------------------------------------------------------------------*/