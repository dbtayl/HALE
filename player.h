#ifndef HALE_PLAYER_H
#define HALE_PLAYER_H

#include "config.h"
#include "state.h"


//Adds a tile to a player's hand. Nominally a simple function, but may
//be more complex for non-PC HALE
HALE_status_t giveTile(GameState_t* gs, uint8_t tile, uint8_t playerNum);


//Calculates the current player cash value
int32_t calculatePlayerValue(GameState_t* gs, uint8_t playerNum);

#endif
