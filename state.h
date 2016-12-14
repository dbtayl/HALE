#ifndef HALE_STATE_H
#define HALE_STATE_H

#include "config.h"
#include "chain.h"


//Forward declaration of GameState_t
typedef struct GameState_s GameState_t;

typedef struct
{
	uint8_t (*playTile)(GameState_t* gs, uint8_t playerNum);
	uint8_t (*formChain)(GameState_t* gs, uint8_t playerNum);
	uint8_t (*mergerSurvivor)(GameState_t* gs, uint8_t playerNum, uint8_t numOptions, uint8_t* options);
	void (*buyStock)(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);
	void (*mergerTrade)(GameState_t* gs, uint8_t playerNum, uint8_t survivor, uint8_t merged, uint8_t* tradeFor, uint8_t* sell);
	uint8_t (*endGame)(GameState_t* gs, uint8_t playerNum);
	char* name;
} PlayerActions_t;

typedef struct
{
	int32_t cash;
	int8_t stocks[NUM_CHAINS];
	uint8_t tiles[HAND_SIZE];
	PlayerActions_t actions;
} Player_t;

struct GameState_s
{
	uint8_t numPlayers;
	chain_t board[BOARD_TILES];
	int8_t remainingStocks[NUM_CHAINS];
	uint8_t tilePool[BOARD_TILES];
	int8_t remainingTiles;
	uint8_t currentPlayer;
	Player_t players[MAX_PLAYERS];
};


//Generates a "sanitized" copy of a game state, to be given to <playerNum>
//This does NOT allocate space for the new game state!
void makeSanitizedGameStateCopy(GameState_t* gs, GameState_t* newgs, uint8_t playerNum);


//This function contains the entire game flow logic- setup, operation, and completion
void runGame(uint8_t numPlayers);

#endif
