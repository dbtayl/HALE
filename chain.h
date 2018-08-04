#ifndef HALE_CHAIN_H
#define HALE_CHAIN_H

#include <stdint.h>

#include "config.h"

typedef struct GameState_s GameState_t;

#define NUM_CHAINS (7)

//How big a chain has to be before it cannot disappear
#define SAFE_CHAIN_SIZE (11)

//How large a chain must be to qualify for ending the game
#define CHAIN_SIZE_GAME_END (41)


//None indicates a tile is in place, but not part of a chain
//empty indicates no tile is in place
//It would in some ways be convenient to be able to bitmask these, but
//they're used as indices into arrays, so... not right now.
//Plus, that would require each to be two bytes.
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


//Returns the bonuses for shareholders
//If <prices> is not NULL, it will be used to calculate the bonuses and save some computation
//Otherwise, getChainPricesPerShare will be used to do that first
HALE_status_t getChainBonuses(GameState_t* gs, int32_t* prices, int32_t* majBonus, int32_t* minBonus);


//Calculates the player's bonus for a given chain
//Could totally be zero
//Player bonus is as follows:
//One majority, one minority holder: Each gets full bonus (10x, 5x, respectively)
//Two+ majority holders: Split sum of majority and minority bonuses (15x total) between them, minority holder(s) get nothing
//One majority, two plus minority: Majority gets full bonus (10x), minority splits
HALE_status_t calculatePlayerBonus(GameState_t* gs, uint8_t playerNum, chain_t chain, int32_t* bonus);


#endif
