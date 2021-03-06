#ifndef HALE_BOARD_H
#define HALE_BOARD_H

#include "config.h"
#include "state.h"


//Macros to get the index of the square above/below/left-of/right-of
//the square given. Returns BOARD_NULL if invalid.
//These should NOT be used internally- there's a lookup table for that.
//If you want your own lookup table, make it yourself! I don't want to deal
//with players possibly hosing the lookup table.
#define SQUARE_UP(x) ( (x >= BOARD_WIDTH) ? (x - BOARD_WIDTH) : BOARD_NULL)
#define SQUARE_DOWN(x) ( (x < (BOARD_TILES - BOARD_WIDTH)) ? (x + BOARD_WIDTH) : BOARD_NULL)
#define SQUARE_LEFT(x) ( ((x % BOARD_WIDTH) > 0) ? (x - 1) : BOARD_NULL)
#define SQUARE_RIGHT(x) ( ((x % BOARD_WIDTH) < (BOARD_WIDTH - 1)) ? (x + 1) : BOARD_NULL)


//Takes an array of tiles, and shuffles them.
//Really just randomizes any array of size BOARD_TILES
HALE_status_t shuffleTiles(uint8_t* tiles);

//If possible, draws a tile out of the pool and returns it in <tile>
//Also decrements the remaining tiles counter
HALE_status_t drawTile(GameState_t* gs, uint8_t* tile);

//Attempts to draw a tile and give it to the player number indicated
HALE_status_t dealTile(GameState_t* gs, uint8_t playerNum);

//Completely wipes out and redeals a players hand
//Useful at the beginning of the game, then also when a player has an
//unplayable hand
HALE_status_t redealTiles(GameState_t* gs, uint8_t playerNum);

//Populates <adjacentSquares> with the values of the squares up, left, right,
//and down from the tile <tile>.
HALE_status_t getAdjacentSquares(GameState_t* gs, uint8_t tile, chain_t* adjacentSquares);


//Returns zero if a tile is invalid to play, non-zero if it's a valid move
uint8_t isValidTilePlay(GameState_t* gs, uint8_t tile);


//Returns the number of tiles a player has that are valid to play
uint8_t getNumValidTiles(GameState_t* gs, uint8_t playerNum);

//Returns zero if playing the specified tile would NOT cause a merger; non-
//zero otherwise
//If not NULL, <numMergingChains> will be filled in with the number of chains
//that would be involved in a potential merger
//If not NULL, <mergingChains> will be filled in with the names of the chains
//that would be involved- note that <mergingChains> must be NUM_CHAINS in size-
//one-hot encoding!
//Note that this does NOT tell you if a merger would actually be legal!
uint8_t wouldCauseMerger(GameState_t* gs, uint8_t tile, uint8_t* numMergingChains, chain_t* mergingChains);

//Returns zero if playing the specified tile would NOT cause a new chain
//to be created. Returns non-zero otherwise.
//Note that this does NOT tell you if creating said chain would be legal!
uint8_t wouldCreateChain(GameState_t* gs, uint8_t tile);


//Returns the sizes of ALL of the chains- there's really no point in
//getting the size of just one- it's equal complexity
HALE_status_t getChainSizes(GameState_t* gs, uint8_t* sizes);


//Expands <tile> on the board, changing all neighboring NON-CHAIN tiles- type CHAIN_NONE only!-
//to the same chain as <tile>
HALE_status_t floodFillNonChain(GameState_t* gs, uint8_t tile);


//Plays a tile- updates the board to include the new tiles, calls appropriate
//functions to handle mergers and whatnot
HALE_status_t playTile(GameState_t* gs, uint8_t tile, uint8_t playerNum);


//Returns non-zero if the game is eligible to be ended
uint8_t canEndGame(GameState_t* gs);


//Prints the board out to the terminal
HALE_status_t printGameBoard(GameState_t* gs);


#endif
