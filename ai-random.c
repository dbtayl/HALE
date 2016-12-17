#include <stdlib.h>

#include "ai-random.h"
#include "board.h"
#include "config.h"
#include "state.h"
#include "util.h"

PlayerActions_t randomActions = {	.playTile = randomPlayTile,
					.formChain = randomFormChain,
					.mergerSurvivor = randomMergerSurvivor,
					.buyStock = randomBuyStock,
					.mergerTrade = randomMergerTrade,
					.endGame = randomEndGame,
					.name = NULL
				};


uint8_t randomPlayTile(GameState_t* gs, uint8_t playerNum)
{
	uint8_t numPlayable = 0;
	uint8_t playableTiles[HAND_SIZE] = {TILE_NULL};
	
	//Get list of valid plays
	int i;
	for(i = 0; i < HAND_SIZE; i++)
	{
		uint8_t tile = gs->players[playerNum].tiles[i];
		if(isValidTilePlay(gs, tile))
		{
			//increment the count of playable tiles AFTER using
			//it as an index
			playableTiles[numPlayable] = tile;
			numPlayable++;
		}
	}
	
	//Throw an error if we get into a state where we can't play ANY tiles
	//The game should stop this from ever happening
	if(numPlayable == 0)
	{
		HANDLE_UNRECOVERABLE_ERROR(HALE_AI_BADSTATE);
	}
	
	//Othewise, pick one at random
	return playableTiles[rand() % numPlayable];
}


chain_t randomFormChain(GameState_t* gs, uint8_t playerNum)
{
	uint8_t chainSizes[NUM_CHAINS];
	getChainSizes(gs, chainSizes);
	
	//Get the list of valid chains
	chain_t validChains[NUM_CHAINS];
	int numValid = 0;
	int i;
	for(i = 0; i < NUM_CHAINS; i++)
	{
		if(chainSizes[i] == 0)
		{
			validChains[numValid] = i;
			numValid++;
		}
	}
	
	//Throw an error if we get into a state where we can't form a chain
	//The game should stop this from ever happening
	if(numValid == 0)
	{
		HANDLE_UNRECOVERABLE_ERROR(HALE_AI_BADSTATE);
	}
	
	
	//Otherwise, pick one at random
	return validChains[rand() % numValid];
}


chain_t randomMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t numOptions, chain_t* options)
{
	//Easy- just pick one of the provided options at random
	return options[rand() % numOptions];
}


void randomBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy)
{
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
}


void randomMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t merged, uint8_t* tradeFor, uint8_t* sell)
{
	//The most stocks we could trade FOR or sell
	//Selling is easy- can sell at most how many you have
	uint8_t maxSell = gs->players[playerNum].stocks[merged];
	
	//How many you could trade FOR could be defined by how many you have,
	//or it could be defined by how many are available to trade for
	uint8_t maxTradeFor = gs->players[playerNum].stocks[merged]/2;
	if(gs->remainingStocks[survivor] < maxTradeFor)
	{
		maxTradeFor = gs->remainingStocks[survivor];
	}
	
	//Randomize if we pick how much to trade or how much to buy first
	//I think this prevents a bias...
	int tradeFirst = rand() % 2;
	
	//Perform first action
	//Add one to modulos so we get a range of 0-n, inclusive of n
	if(tradeFirst)
	{
		*tradeFor = rand() % (maxTradeFor + 1);
	}
	else
	{
		*sell = rand() % (maxSell + 1);
	}
	
	//Perform second action
	if(tradeFirst)
	{
		//Again, selling is always limited by how many you have
		*sell = rand() % (maxSell - *tradeFor*2 + 1);
	}
	else
	{
		//Before trading, need to re-check limits
		uint8_t possibleNewLimit = (gs->players[playerNum].stocks[merged] - *sell)/2;
		if(possibleNewLimit < maxTradeFor)
		{
			maxTradeFor = possibleNewLimit;
		}
		*tradeFor = rand() % (maxTradeFor + 1);
	}
}


uint8_t randomEndGame(GameState_t* gs, uint8_t playerNum)
{
	//literally pick a random 0 or 1
	return (uint8_t)(rand() & 0x01);
}
