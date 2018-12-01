#ifndef HALE_AI_HUMAN_H
#define HALE_AI_HUMAN_H


#include <stdint.h>

#include "state.h"

/*
 * This "ai" is a shim for a human player- it requests human input, then
 * plays the requested moves.
 *
 * To determine: If any error checking is done. Initially, probably not.
 */


//See player.h for an explanation of these functions

extern const PlayerActions_t humanActions;



uint8_t humanPlayTile(GameState_t* gs, uint8_t playerNum);

chain_t humanFormChain(GameState_t* gs, uint8_t playerNum);

chain_t humanMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void humanMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void humanBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void humanMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t humanEndGame(GameState_t* gs, uint8_t playerNum);


#endif
