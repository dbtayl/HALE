#include <stdio.h>

#include "config.h"
#include "player.h"
#include "state.h"
#include "util.h"
#include "chain.h"

HALE_status_t giveTile(GameState_t* gs, uint8_t tile, uint8_t playerNum)
{
	CHECK_NULL_PTR(gs, "gs");
	
	if(playerNum >= gs->numPlayers)
	{
		PRINT_MSG("playerNum out of bounds");
		return HALE_OOB;
	}
	
	for(int i = 0; i < HAND_SIZE; i++)
	{
		if(gs->players[playerNum].tiles[i] == TILE_NULL)
		{
			gs->players[playerNum].tiles[i] = tile;
			return HALE_OK;
		}
	}
	
	PRINT_MSG("Tried to give a tile to a player with a full hand!");
	return HALE_PLAYER_HAND_FULL;
}



HALE_status_t calculatePlayerValue(GameState_t* gs, uint8_t playerNum, int32_t* val)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(val, "val");
	
	if(playerNum >= gs->numPlayers)
	{
		PRINT_MSG("playerNum out of bounds");
		return HALE_OOB;
	}
	
	//Base value is just cash
	Player_t* p = &(gs->players[playerNum]);
	*val = p->cash;
	
	//Plus value of any held stock
	int32_t sharePrices[NUM_CHAINS];
	uint8_t chainSizes[NUM_CHAINS];
	getChainPricesPerShare(gs, sharePrices, chainSizes); //FIXME: Error checking
	
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		*val += p->stocks[i] * sharePrices[i];
	}
	
	//Get all bonuses
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		int32_t bonus;
		calculatePlayerBonus(gs, playerNum, i, &bonus); //FIXME: Error checking
		*val += bonus;
	}

	return HALE_OK;
}

void printPlayer(GameState_t* gs, uint8_t playerNum)
{
	Player_t* player = &(gs->players[playerNum]);
	//FIXME: Trying to avoid printfs...
	fprintf(stdout, "Player %d (%s)\n", playerNum, player->name);
	fprintf(stdout, "$%d\n", player->cash);
	fprintf(stdout, "Tiles: ");
	for(int i = 0; i < HAND_SIZE; i++)
	{
		fprintf(stdout, "%d ", player->tiles[i]);
	}
	fprintf(stdout, "\nStocks: ");
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		fprintf(stdout, "%d ", player->stocks[i]);
	}
	fprintf(stdout, "\n");
	
	int32_t totalValue;
	calculatePlayerValue(gs, playerNum, &totalValue);
	fprintf(stdout, "Total value: $%d\n", totalValue);
}
