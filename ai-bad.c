#include <stdlib.h>

#include "ai-bad.h"
#include "board.h"
#include "config.h"
#include "state.h"
#include "util.h"

PlayerActions_t badActions = {	.playTile = badPlayTile,
					.formChain = badFormChain,
					.mergerSurvivor = badMergerSurvivor,
					.mergerOrder = badMergerOrder,
					.buyStock = badBuyStock,
					.mergerTrade = badMergerTrade,
					.endGame = badEndGame
				};


uint8_t badPlayTile(GameState_t* gs, uint8_t playerNum)
{
	//Return ANY number between 0 and 255, valid or not
	return (rand() & 0xff);
}


chain_t badFormChain(GameState_t* gs, uint8_t playerNum)
{
	//Return ANY random integer, almost certainly NOT a valid response
	return rand();
}


chain_t badMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options)
{
	//Return ANY random integer, almost certainly NOT a valid response
	return rand();
}


void badMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options)
{
	//Multiple ways we could try to break things:
	//-Duplicate orders
	//-Including merging chains that don't belong
	//-NOT including merging chains that DO belong
	
	//...and all of those can be accomplished, statistically, by
	//just randomly filling in <options>
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		options[i] = rand() & 0xff;
	}
}


void badBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy)
{
	//Here we probably want to be a little more structured... want
	//to cover:
	//-Buying defunct chains
	//-Buying chains with no remaining shares
	//-Buying more than 3 stocks
	//-Buying more than the player can afford
	
	//FIXME: Pure random for the moment
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		toBuy[i] = rand() & 0xff;
	}
}


void badMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell)
{
	//FIXME: Pure random for the moment
	*tradeFor = rand() & 0xff;
	*sell = rand() & 0xff;
}


uint8_t badEndGame(GameState_t* gs, uint8_t playerNum)
{
	//Try to force the game to never end
	return 0;
}
