#ifndef HALE_AI_RANDOM_H
#define HALE_AI_RANDOM_H

#include <stdint.h>

#include "state.h"

/*
 * This AI is random- every move is randomized from the set of valid options,
 * with no strategy whatsoever.
 */


//See player.h for an explanation of these functions

extern PlayerActions_t randomActions;



uint8_t randomPlayTile(GameState_t* gs, uint8_t playerNum);

chain_t randomFormChain(GameState_t* gs, uint8_t playerNum);

chain_t randomMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void randomMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void randomBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void randomMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t randomEndGame(GameState_t* gs, uint8_t playerNum);


#endif
