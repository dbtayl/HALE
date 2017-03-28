#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "board.h"
#include "player.h"
#include "util.h"


//Lookup table for adjacent tiles
static const uint8_t squareAdjacencies[][4] = {
	{BOARD_NULL, BOARD_NULL, 1, 12},
	{BOARD_NULL, 0, 2, 13},
	{BOARD_NULL, 1, 3, 14},
	{BOARD_NULL, 2, 4, 15},
	{BOARD_NULL, 3, 5, 16},
	{BOARD_NULL, 4, 6, 17},
	{BOARD_NULL, 5, 7, 18},
	{BOARD_NULL, 6, 8, 19},
	{BOARD_NULL, 7, 9, 20},
	{BOARD_NULL, 8, 10, 21},
	{BOARD_NULL, 9, 11, 22},
	{BOARD_NULL, 10, BOARD_NULL, 23},
	{0, BOARD_NULL, 13, 24},
	{1, 12, 14, 25},
	{2, 13, 15, 26},
	{3, 14, 16, 27},
	{4, 15, 17, 28},
	{5, 16, 18, 29},
	{6, 17, 19, 30},
	{7, 18, 20, 31},
	{8, 19, 21, 32},
	{9, 20, 22, 33},
	{10, 21, 23, 34},
	{11, 22, BOARD_NULL, 35},
	{12, BOARD_NULL, 25, 36},
	{13, 24, 26, 37},
	{14, 25, 27, 38},
	{15, 26, 28, 39},
	{16, 27, 29, 40},
	{17, 28, 30, 41},
	{18, 29, 31, 42},
	{19, 30, 32, 43},
	{20, 31, 33, 44},
	{21, 32, 34, 45},
	{22, 33, 35, 46},
	{23, 34, BOARD_NULL, 47},
	{24, BOARD_NULL, 37, 48},
	{25, 36, 38, 49},
	{26, 37, 39, 50},
	{27, 38, 40, 51},
	{28, 39, 41, 52},
	{29, 40, 42, 53},
	{30, 41, 43, 54},
	{31, 42, 44, 55},
	{32, 43, 45, 56},
	{33, 44, 46, 57},
	{34, 45, 47, 58},
	{35, 46, BOARD_NULL, 59},
	{36, BOARD_NULL, 49, 60},
	{37, 48, 50, 61},
	{38, 49, 51, 62},
	{39, 50, 52, 63},
	{40, 51, 53, 64},
	{41, 52, 54, 65},
	{42, 53, 55, 66},
	{43, 54, 56, 67},
	{44, 55, 57, 68},
	{45, 56, 58, 69},
	{46, 57, 59, 70},
	{47, 58, BOARD_NULL, 71},
	{48, BOARD_NULL, 61, 72},
	{49, 60, 62, 73},
	{50, 61, 63, 74},
	{51, 62, 64, 75},
	{52, 63, 65, 76},
	{53, 64, 66, 77},
	{54, 65, 67, 78},
	{55, 66, 68, 79},
	{56, 67, 69, 80},
	{57, 68, 70, 81},
	{58, 69, 71, 82},
	{59, 70, BOARD_NULL, 83},
	{60, BOARD_NULL, 73, 84},
	{61, 72, 74, 85},
	{62, 73, 75, 86},
	{63, 74, 76, 87},
	{64, 75, 77, 88},
	{65, 76, 78, 89},
	{66, 77, 79, 90},
	{67, 78, 80, 91},
	{68, 79, 81, 92},
	{69, 80, 82, 93},
	{70, 81, 83, 94},
	{71, 82, BOARD_NULL, 95},
	{72, BOARD_NULL, 85, 96},
	{73, 84, 86, 97},
	{74, 85, 87, 98},
	{75, 86, 88, 99},
	{76, 87, 89, 100},
	{77, 88, 90, 101},
	{78, 89, 91, 102},
	{79, 90, 92, 103},
	{80, 91, 93, 104},
	{81, 92, 94, 105},
	{82, 93, 95, 106},
	{83, 94, BOARD_NULL, 107},
	{84, BOARD_NULL, 97, BOARD_NULL},
	{85, 96, 98, BOARD_NULL},
	{86, 97, 99, BOARD_NULL},
	{87, 98, 100, BOARD_NULL},
	{88, 99, 101, BOARD_NULL},
	{89, 100, 102, BOARD_NULL},
	{90, 101, 103, BOARD_NULL},
	{91, 102, 104, BOARD_NULL},
	{92, 103, 105, BOARD_NULL},
	{93, 104, 106, BOARD_NULL},
	{94, 105, 107, BOARD_NULL},
	{95, 106, BOARD_NULL, BOARD_NULL}
};


//Not a particularly smart algorithm; for each tile index, picks a random
//different tile index and swaps the two
HALE_status_t shuffleTiles(uint8_t* tiles)
{
	CHECK_NULL_PTR_FATAL(tiles, "tiles");
	
	int i;
	for(i = 0; i < BOARD_TILES; i++)
	{
		int j = rand() % BOARD_TILES;
		uint8_t tmp = tiles[i];
		tiles[i] = tiles[j];
		tiles[j] = tmp;
	}
	return HALE_OK;
}


//If possible, draws a tile out of the pool and returns it in <tile>
//Also decrements the remaining tiles counter
HALE_status_t drawTile(GameState_t* gs, uint8_t* tile)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(tile, "tile");
	
	*tile = TILE_NULL;
	 
	if(gs->remainingTiles > 0)
	{
		//If we decrement remainingTiles BEFORE loading the tile to return, we don't need to add a "-1"...
		gs->remainingTiles--;
		*tile = gs->tilePool[gs->remainingTiles];
		gs->tilePool[gs->remainingTiles] = TILE_NULL;
	}
	//If there were no tiles left, return such
	else
	{
		return HALE_TILE_POOL_EMPTY;
	}

	//Otherwise, if there WERE tiles left, BUT we are STILL somehow go
	//a TILE_NULL, return an error
	//Similarly, if we just drew a tile that's already on the board, that's
	//an error
	if( (*tile == TILE_NULL) || (gs->board[*tile] != TILE_NULL) )
	{
		return HALE_TILE_POOL_CORRUPTED;
	}
	
	//If another player already has this tile, that's a problem
	//Don't bother checking against NULL here- we caught the NULL case above
#ifdef ENABLE_PARANOID_CHECKS
	{
		int i,j;
		for(i = 0; i < gs->numPlayers; i++)
		{
			for(j = 0; j < HAND_SIZE; j++)
			{
				if(gs->players[i].tiles[j] == *tile)
				{
					return HALE_TILE_POOL_CORRUPTED;
				}
			}
		}
	}
#endif
	 
	 //We found a tile and no errors
	 return HALE_OK;
}


HALE_status_t dealTile(GameState_t* gs, uint8_t playerNum)
{
	CHECK_NULL_PTR(gs,"gs");
	
	//Check if playerNum is in bounds
#ifndef GO_FAST_AND_BREAK_THINGS
	if(playerNum >= gs->numPlayers)
	{
		PRINT_MSG_INT("playerNum is invalid", playerNum);
		//HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
		return HALE_OOB;
	}
#endif //GO_FAST_AND_BREAK_THINGS
	
	uint8_t newTile;
	HALE_status_t err_code = drawTile(gs, &newTile);
	
	//Try to draw a tile- if that fails, check why
	if(err_code != HALE_OK)
	{
		//If there just aren't any tiles left... well, at least let us know
		if(err_code == HALE_TILE_POOL_EMPTY)
		{
			PRINT_MSG("No tiles left in pool");
		}
		//Otherwise, something really bad happened; go to error handler
		else
		{
			HANDLE_UNRECOVERABLE_ERROR(err_code);
		}
	}
	 
	//Try to give the tile to the player
	err_code = giveTile(gs, newTile, playerNum);
	if(err_code != HALE_OK)
	{
		if(err_code == HALE_PLAYER_HAND_FULL)
		{
			PRINT_MSG("Tried to give a tile to a player with a full hand");
		}
		HANDLE_UNRECOVERABLE_ERROR(err_code);
	}
	 
	return HALE_OK;
}


//Populates <adjacentSquares> with the values of the squares up, left, right,
//and down from the tile <tile>.
HALE_status_t getAdjacentSquares(GameState_t* gs, uint8_t tile, chain_t* adjacentSquares)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(adjacentSquares, "adjacentSquares");
	
	//Resolve to state of each square and count up how many uniqe tile types
	int i;
	for(i = 0; i < 4; i++)
	{
		uint8_t idx = squareAdjacencies[tile][i];
		adjacentSquares[i] = (idx == BOARD_NULL) ? CHAIN_EMPTY : gs->board[idx];
	}
	
	return HALE_OK;
}


uint8_t isValidTilePlay(GameState_t* gs, uint8_t tile)
{
	//Invalid if:
	//-would merge two safe chains
	//-would create a chain when there are already 7 in play
	//-tile is already on the board
	
	
	//Not valid if games state isn't valid
	if(gs == NULL)
	{
		PRINT_MSG("gs invalid -> tile invalid");
		return 0;
	}
	
	//Firstly, if the TILE isn't valid, it can't be a valid play
	//These are arguably redundant checks, but it seems like a good
	//practice to have both of them in there
	//The last check really shouldn't be necessary, but it's also technically
	//an invalid more to play a tile that's already in play, so...
	if( (tile >= BOARD_TILES) || (tile == TILE_NULL) || gs->board[tile] != CHAIN_EMPTY)
	{
		PRINT_MSG_INT("tile OOB... or something like that. tile was", tile);
		return 0;
	}
	
	chain_t mergingChains[4];
	uint8_t numMergingChains;
	uint8_t merge = wouldCauseMerger(gs, tile, &numMergingChains, mergingChains);
	uint8_t create = wouldCreateChain(gs, tile);
	
	//If it doesn't cause a merger or create a chain, it must be OK
	if(!merge && !create)
	{
		return 1;
	}
	
	//Otherwise, we need to know how big the chains are
	uint8_t sizes[NUM_CHAINS];
	getChainSizes(gs, sizes);
	
	//If this would cause a merger, make sure the merger is legal
	if(merge)
	{
		//Can only be illegal if trying to merge two or more safe chains
		uint8_t safeCount = 0;
		int i;
		for(i = 0; i < numMergingChains; i++)
		{
			if(sizes[mergingChains[i]] >= SAFE_CHAIN_SIZE)
			{
				safeCount++;
			}
		}
		
		//If two or more are safe, it's an illegal move
		if(safeCount > 1)
		{
			PRINT_MSG_INT("tile would merge two safe chains. tile", tile);
			return 0;
		}
	}
	
	//If this would create a chain, make sure creating a chain is legal
	if(create)
	{
		uint8_t legal = 0;
		
		//Check if any chain is of size 0
		int i;
		for(i = 0; i < NUM_CHAINS; i++)
		{
			if(sizes[i] == 0)
			{
				legal = 1;
				break;
			}
		}
		
		//if not, return invalid move
		if(!legal)
		{
			PRINT_MSG_INT("tile would create a new chain, but no new chains available. tile", tile);
			return 0;
		}
	}
	
	
	//If it's not illegal, it must be legal
	return 1;
}


uint8_t wouldCauseMerger(GameState_t* gs, uint8_t tile, uint8_t* numMergingChains, chain_t* mergingChains)
{
	//NOTE: Don't bother checking if the tile has already been played-
	//the return value should be valid regardless
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(numMergingChains, "numMergingChains");
	CHECK_NULL_PTR(mergingChains, "mergingChains");
	
	//Step 0: Zero out the count of merging chains
	*numMergingChains = 0;
	
	//Step 1: Check what the squares around the proposed play location
	//contain
	chain_t adj[4];
	getAdjacentSquares(gs, tile, adj);
	
	//Step 2: see if there's more than one chain contained in that set
	int i;
	for(i = 0; i < 4; i++)
	{
		//for each chain, check if it matches one of the already-recorded chains...
		if(adj[i] < CHAIN_NONE)
		{
			int j;
			uint8_t dup = 0;
			for(j = 0; j < *numMergingChains; j++)
			{
				if(adj[i] == mergingChains[j])
				{
					dup = 1;
				}
			}
			
			//...if not, record it
			if(!dup)
			{
				mergingChains[*numMergingChains] = adj[i];
				*numMergingChains = (*numMergingChains) + 1;
			}
		} //if(adj[u[ < CHAIN_NONE)
	}//for(i = 0; i < 4; i++)
	
	
	//Step 3: Return 1 if more than one chain is immediately-adjacent
	return (*numMergingChains > 0) ? 1 : 0;
}


uint8_t wouldCreateChain(GameState_t* gs, uint8_t tile)
{
	//NOTE: Don't bother checking if the tile has already been played-
	//the return value should be valid regardless
	CHECK_NULL_PTR(gs, "gs");
	
	if(tile >= BOARD_TILES)
	{
		PRINT_MSG_INT("<tile> is out of bounds", tile);
		return HALE_OOB;
	}
	
	//First, find out what tiles types are adjacent
	//Get indices of board adjacent to tile: Up, left, right, down
	int i;
	uint8_t nonChainAdjacent = 0;
	chain_t adj[4];
	//FIXME: Check return value
	getAdjacentSquares(gs, tile, adj);
	
	//Resolve to state of each square and check for adjacent tiles; if any
	//chain tiles are adjacent, can return false immediately
	for(i = 0; i < 4; i++)
	{
		//We need at LEAST ONE non-chain tile adjacent to form a new chain
		if(adj[i] == CHAIN_NONE)
		{
			nonChainAdjacent = 1;
		}
		//If we have ANY chain tiles adjacent, we're not making a new chain
		else if(adj[i] < CHAIN_NONE)
		{
			return 0;
		}
	}
	
	//If we get here, it's just a matter of checking if we've got any
	//non-chain adjacent tiles; if yes, we'd form a new chain. If no, we
	//wouldn't
	return nonChainAdjacent;
}


HALE_status_t getChainSizes(GameState_t* gs, uint8_t* sizes)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(sizes, "sizes");
	
	memset(sizes, 0, NUM_CHAINS);
	
	//brute-force through the board to calculate chain sizes
	int i;
	for(i = 0; i < BOARD_TILES; i++)
	{
		chain_t c = gs->board[i];
		if(c < CHAIN_NONE)
		{
			sizes[c]++;
		}
	}
	
	return HALE_OK;
}


//Expands <tile> on the board, changing all neighboring NON-CHAIN tiles- type CHAIN_NONE only!-
//to the same chain as <tile>
//At MOST, there are numPlayers + 3 tiles to change:
//-<= numPlayers tiles from initial placement (nominally to determine turn order)
//-3 other tiles surrounding said tile.
//Plus one more for the tile itself
#ifndef MAX_FLOOD_CHECK_SIZE
#define MAX_FLOOD_CHECK_SIZE (MAX_PLAYERS + 3 + 1)
#endif
HALE_status_t floodFillNonChain(GameState_t* gs, uint8_t tile)
{
	PRINT_MSG("START");
	CHECK_NULL_PTR(gs, "gs");
	
	//If we got a bad tile passed in, fail gracefully
	if(tile >= BOARD_TILES)
	{
		PRINT_MSG_INT("Tile OOB", tile);
		return HALE_OOB;
	}
	
	//If the tile we're referring to isn't part of a chain, that's not helpful
	if(gs->board[tile] >= CHAIN_NONE)
	{
		PRINT_MSG_INT("Specified base tile isn't a chain", gs->board[tile]);
		return HALE_BAD_INPUT;
	}
	
	
	uint8_t checkIdx = 0;
	uint8_t squaresToCheck[MAX_FLOOD_CHECK_SIZE] = {TILE_NULL};
	
	uint8_t tmpTile;
	
	//Initially populate the "check list" with the tile itself
	squaresToCheck[checkIdx] = tile;
	checkIdx++;
	int i;
	
	for(i = 0; i < checkIdx; i++)
	{
		PRINT_MSG_INT("iterate", checkIdx);
		tmpTile = squaresToCheck[i];
#ifndef GO_FAST_AND_BREAK_THINGS
		//If we're trying to flood-fill next to a DIFFERENT chain,
		//something is wrong
		if( (gs->board[tmpTile] < CHAIN_NONE) && (gs->board[tmpTile] != gs->board[tile]) )
		{
			PRINT_MSG("Tried to flood fill next to a different chain");
			return HALE_BOARD_CORRUPTED;
		}
#endif
		
		//If this is a tile to expand into, do so, then add the
		//surrounding tiles as tiles to investigate... assuming
		//they're not already checked or on the list to be checked
		//FIXME: The second part of this expression is just a hack
		//to make it evaluate the first tile- should really fix the
		//logic/flow instead.
		PRINT_MSG_INT("Type of tile we're looking at", gs->board[tmpTile]);
		if( (gs->board[tmpTile] == CHAIN_NONE) || (tmpTile == tile) )
		{
			//Change state of current tile
			gs->board[tmpTile] = gs->board[tile];
			
			//Check adjacent tiles
			int k;
			for(k = 0; k < 4; k++)
			{
				//Check if the tile was already on the list
				uint8_t tmpAdj = squareAdjacencies[tmpTile][k];
				if(tmpAdj != BOARD_NULL)
				{
					uint8_t add = 1;
					int j;
					for(j = 0; j < checkIdx; j++)
					{
						if(tmpAdj == squaresToCheck[j])
						{
							add = 0;
							break;
						}
					}
					
					//If the square wasn't already on the list, add it
					if(add)
					{
						PRINT_MSG_INT("Adding tile to to-check list", tmpAdj);
#ifndef GO_FAST_AND_BREAK_THINGS
						if(checkIdx == MAX_FLOOD_CHECK_SIZE)
						{
							PRINT_MSG("Too many adjacent non-chain tiles!");
							return HALE_SHOULD_BE_IMPOSSIBLE;
						}
#endif
						squaresToCheck[checkIdx] = tmpAdj;
						checkIdx++;
					}
				} //if(squareAdjacencies[tmpTile][i] != BOARD_NULL)
			} //for(i = 0; i < 4; i++)
		} //if(gs->board[tmpTile] == CHAIN_NONE)
		//If it's not a non-chain tile, AND it's not empty, AND it's not
		//the same as the current chain, we shouldn't be calling this function
		else if( (gs->board[tmpTile] != CHAIN_EMPTY) && (gs->board[tmpTile] != gs->board[tile]) )
		{
			PRINT_MSG("Tried to flood fill into a different chain");
			return HALE_SHOULD_BE_IMPOSSIBLE;
		}
		PRINT_MSG_INT("checkIdx", checkIdx);
	} //for(i = 0; i < checkIdx; i++)
	PRINT_MSG("END");
	
	return HALE_OK;
}


HALE_status_t playTile(GameState_t* gs, uint8_t tile, uint8_t playerNum)
{
	CHECK_NULL_PTR(gs, "gs");
	
	HALE_status_t err_code = HALE_OK;
	
	//Always make sure it's a valid move in the first place
	if(!isValidTilePlay(gs, tile))
	{
		PRINT_MSG_INT("Bad tile play requested", tile);
		return HALE_BAD_INPUT;
	}
	
	uint8_t numMergingChains = 0;
	chain_t mergingChains[NUM_CHAINS]; //FIXME: Technically only 4 chains could merge at once, so this is overkill
	uint8_t merger = wouldCauseMerger(gs, tile, &numMergingChains, mergingChains);
	uint8_t create = wouldCreateChain(gs, tile);
	
#ifndef GO_FAST_AND_BREAK_THINGS
	//Should never have a merge AND create a chain at the same time
	if(merger && create)
	{
		PRINT_MSG("ERROR: Both merger and creating a chain?!?!?");
		return HALE_SHOULD_BE_IMPOSSIBLE;
	}
#endif
	
	//One or more mergers to be handled
	if(merger)
	{
		PRINT_MSG_INT("Tile would cause merger", tile);
		//remember to pass a COPY of gs!
		GameState_t newgs;
		err_code = makeSanitizedGameStateCopy(&newgs, gs, playerNum);
		if(err_code != HALE_OK)
		{
			PRINT_MSG("Couldn't create sanitized state for handling merger");
			return err_code;
		}
		PRINT_MSG("FIXME: Need to request merger order from player");
		PRINT_MSG("FIXME: Probably call a different function to handle mergers");
	}
	//Need to make a new chain
	else if(create)
	{
		PRINT_MSG_INT("Tile would create new chain", tile);
		//remember to pass a COPY of gs!
		GameState_t newgs;
		err_code = makeSanitizedGameStateCopy(&newgs, gs, playerNum);
		if(err_code != HALE_OK)
		{
			PRINT_MSG("Couldn't create sanitized state for creating chain");
			return err_code;
		}
		
		//Request chain to form from the player
		chain_t chainToCreate = gs->players[playerNum].actions.formChain(&newgs, playerNum);
		
		//Before doing anything, validate that the requested chain can, in fact, be created
		uint8_t chainSizes[NUM_CHAINS];
		getChainSizes(gs, chainSizes);
		if(chainSizes[chainToCreate] > 0)
		{
			PRINT_MSG_INT("Requested to form invalid chain; picking one for them... requested", chainToCreate);
			int i;
			for(i = 0; i < NUM_CHAINS; i++)
			{
				if(chainSizes[i] == 0)
				{
					chainToCreate = i;
					break;
				}
			}
#ifdef ENABLE_PARANOID_CHECKS
			if(i >= NUM_CHAINS)
			{
				PRINT_MSG("Should-be-impossible: Can't find chain to form, but tile identified as valid to play");
				HANDLE_UNRECOVERABLE_ERROR(HALE_SHOULD_BE_IMPOSSIBLE);
			}
#endif
		}
		
		PRINT_MSG("FIXME: Need to act on said request");
		gs->board[tile] = chainToCreate;
		floodFillNonChain(gs, tile);
	}
	//Tile touches nothing, or tile touches a chain, or tile touches
	//a chain and one or more non-chain tiles
	else
	{
		chain_t adj[4];
		getAdjacentSquares(gs, tile, adj);
		chain_t newType = CHAIN_NONE;
		
		//Figure out what chain this tile should be a part of
		int i;
		for(i = 0; i < 4; i++)
		{
			if(adj[i] < CHAIN_NONE)
			{
				newType = adj[i];
			}
#ifdef ENABLE_PARANOID_CHECKS
			//Check if we SHOULD have made a new chain
			if(adj[i] == CHAIN_NONE)
			{
				PRINT_MSG("ERROR: Should have made a new chain, but didn't!");
				return HALE_SHOULD_BE_IMPOSSIBLE;
			}
#endif
		}
		
		//Assign the square to the proper chain
		gs->board[tile] = newType;
		
		//Update connected tiles with the new chain, if there is one
		if(newType != CHAIN_NONE)
		{
			err_code = floodFillNonChain(gs, tile);
			if(err_code != HALE_OK)
			{
				PRINT_MSG("Flood fill failed");
				return err_code;
			}
		}
	}
	
	PRINT_MSG("FIXME: Need to update the board, based on mergers and stuff?");
	
	//FIXME
	//HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
	
	return err_code;
}


//Game is eligible to end iff:
//-any one chain is 41+ tiles (CHAIN_SIZE_GAME_END)
//-ALL chains in play are safe (SAFE_CHAIN_SIZE or greater)
uint8_t canEndGame(GameState_t* gs)
{
	CHECK_NULL_PTR(gs, "gs");
	
	uint8_t allChainsSafe = 1;
	uint8_t someChainBigEnough = 0;
	uint8_t chainSizes[NUM_CHAINS];
	
	getChainSizes(gs, chainSizes);
	PRINT_MSG("FIXME: Should check if getChainSizes succeeded");
	
	int i;
	for(i = 0; i < NUM_CHAINS; i++)
	{
		if(chainSizes[i] < SAFE_CHAIN_SIZE)
		{
			allChainsSafe = 0;
		}
		else if(chainSizes[i] >= CHAIN_SIZE_GAME_END)
		{
			someChainBigEnough = 1;
		}
	}
	
	return (allChainsSafe | someChainBigEnough);
}


//CLI representations of each chain
const char* chainStrings[NUM_CHAINS+2] = {ANSI_COLOR_BRIGHT_RED"L"ANSI_COLOR_RESET,
					ANSI_COLOR_BRIGHT_YELLOW"T"ANSI_COLOR_RESET, 
					ANSI_COLOR_YELLOW"W"ANSI_COLOR_RESET,
					ANSI_COLOR_BRIGHT_BLUE"A"ANSI_COLOR_RESET,
					ANSI_COLOR_GREEN"F"ANSI_COLOR_RESET,
					ANSI_COLOR_BRIGHT_MAGENTA"I"ANSI_COLOR_RESET,
					ANSI_COLOR_BRIGHT_CYAN"C"ANSI_COLOR_RESET,
					"#"ANSI_COLOR_RESET,
					"."ANSI_COLOR_RESET
				};

HALE_status_t printGameBoard(GameState_t* gs)
{
	CHECK_NULL_PTR(gs, "gs");
	
	int i, j;
	for(i = 0; i < BOARD_HEIGHT; i++)
	{
		for(j = 0; j < BOARD_WIDTH; j++)
		{
			chain_t tileType = gs->board[i*BOARD_WIDTH+j];
			if(tileType == CHAIN_EMPTY)
			{
				tileType = CHAIN_NONE+1;
			}
				
			printf("%s ", chainStrings[tileType]);
		}
		printf("\n");
	}
	
	printf("\n");
	
	return HALE_OK;
}

