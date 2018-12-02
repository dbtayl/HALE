#ifndef HALE_AI_DAN_H
#define HALE_AI_DAN_H

#include <stdint.h>
#include <string.h>

#include "player.h"
#include "state.h"

/*
 * This is Dan's test AI.
 */


//See player.h for an explanation of these functions

extern const PlayerActions_t danActions;



uint8_t danPlayTile(GameState_t* gs, uint8_t playerNum);

chain_t danFormChain(GameState_t* gs, uint8_t playerNum);

chain_t danMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void danMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void danBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void danMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t danEndGame(GameState_t* gs, uint8_t playerNum);


#endif //HALE_AI_DAN_H
