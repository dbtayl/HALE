#ifndef HALE_CHAIN_H
#define HALE_CHAIN_H

#include <stdint.h>

#include "config.h"

typedef struct GameState_s GameState_t;

#define NUM_CHAINS (7)
#define SAFE_CHAIN_SIZE (11)

//None indicates a tile is in place, but not part of a chain
//empty indicates no tile is in place
typedef enum 	{	CHAIN_LUXOR = 0,
					CHAIN_TOWER,
					CHAIN_WORLDWIDE,
					CHAIN_AMERICAN,
					CHAIN_FESTIVAL,
					CHAIN_IMPERIAL,
					CHAIN_CONTINENTAL,
					CHAIN_NONE,
					CHAIN_EMPTY = 255
				} chain_t;

extern const char* chainNames[];

extern const char* chainConsoleColors[];


//Returns the price of a single share of the given chain
//Will return 0 for invalid chains
//If sizes is not NULL, it will be filled with the size of each chain.
//This is calculated anyway, so it might as well be exposed here.
HALE_status_t getChainPricesPerShare(GameState_t* gs, int32_t* prices, uint8_t* sizesOut);

#endif
