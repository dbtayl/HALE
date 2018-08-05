#ifndef HALE_AI_GREEDY_H
#define HALE_AI_GREEDY_H

#include <stdint.h>

#include "state.h"

/*
 * This AI is greedy- picks the move at every stage that benefits it the
 * most at that instant. Or at least approximately.
 * 
 * It does NOT look at value relative to other players.
 */


//See player.h for an explanation of these functions

extern PlayerActions_t greedyActions;



uint8_t greedyPlayTile(GameState_t* gs, uint8_t playerNum);

chain_t greedyFormChain(GameState_t* gs, uint8_t playerNum);

chain_t greedyMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void greedyMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void greedyBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void greedyMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t merged, uint8_t* tradeFor, uint8_t* sell);

uint8_t greedyEndGame(GameState_t* gs, uint8_t playerNum);

#endif //HALE_AI_GREEDY_H
