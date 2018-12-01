#ifndef HALE_AI_BAD_H
#define HALE_AI_BAD_H

#include <stdint.h>

#include "state.h"

/*
 * This AI is supposed to be bad- not in performance (though certainly
 * that too), but in terms of misbehaving and giving all kinds of bad
 * outputs. It's designed to test the main game engine.
 * 
 * In general, this means unbounded random behavior- in theory that will
 * cover more cases than actually trying to give incorrect outputs in a
 * structured manner.
 */


//See player.h for an explanation of these functions

extern const PlayerActions_t badActions;



uint8_t badPlayTile(GameState_t* gs, uint8_t playerNum);

chain_t badFormChain(GameState_t* gs, uint8_t playerNum);

chain_t badMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void badMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void badBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void badMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t badEndGame(GameState_t* gs, uint8_t playerNum);


#endif
