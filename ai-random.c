#include <stdlib.h>

#include "ai-random.h"
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
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
	return 0; //FIXME: Shut up compiler at least
}


chain_t randomFormChain(GameState_t* gs, uint8_t playerNum)
{
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
	return CHAIN_EMPTY; //FIXME: Shut up compiler at least
}


chain_t randomMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t numOptions, chain_t* options)
{
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
	return CHAIN_EMPTY; //FIXME: Shut up compiler at least
}


void randomBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy)
{
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
}


void randomMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t merged, uint8_t* tradeFor, uint8_t* sell)
{
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
}


uint8_t randomEndGame(GameState_t* gs, uint8_t playerNum)
{
	//literally pick a random 0 or 1
	return (uint8_t)(rand() & 0x01);
}
