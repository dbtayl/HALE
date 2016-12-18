#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "board.h"
#include "player.h"
#include "util.h"


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
	
	uint8_t indices[4] = {SQUARE_UP(tile), SQUARE_LEFT(tile), SQUARE_RIGHT(tile), SQUARE_DOWN(tile)};
	
	//Resolve to state of each square and count up how many uniqe tile types
	int i;
	for(i = 0; i < 4; i++)
	{
		uint8_t idx = indices[i];
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
		return 0;
	}
	
	//Firstly, if the TILE isn't valid, it can't be a valid play
	//These are arguably redundant checks, but it seems like a good
	//practice to have both of them in there
	//The last check really shouldn't be necessary, but it's also technically
	//an invalid more to play a tile that's already in play, so...
	if( (tile >= BOARD_TILES) || (tile == TILE_NULL) || gs->board[tile] != CHAIN_EMPTY)
	{
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
	uint8_t indices[4] = {SQUARE_LEFT(tile), SQUARE_RIGHT(tile), SQUARE_UP(tile), SQUARE_DOWN(tile)};
	uint8_t nonChainAdjacent = 0;
	
	//Resolve to state of each square and check for adjacent tiles; if any
	//chain tiles are adjacent, can return false immediately
	for(i = 0; i < 4; i++)
	{
		uint8_t idx = indices[i];
		chain_t c = (idx == BOARD_NULL) ? CHAIN_EMPTY : gs->board[idx];
		
		//We need at LEAST ONE non-chain tile adjacent to form a new chain
		if(c == CHAIN_NONE)
		{
			nonChainAdjacent = 1;
		}
		//If we have ANY chain tiles adjacent, we're not making a new chain
		else if(c < CHAIN_NONE)
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


//Expands <tile> on the board, changing all neighboring non-chain tiles
//to the same chain as <tile>
HALE_status_t floodFill(GameState_t* gs, uint8_t tile)
{
	//FIXME: Finish
	return HALE_FUNC_NOT_IMPLEMENTED;
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
		//remember to pass a COPY of gs!
		PRINT_MSG("FIXME: Need to request merger order from player");
		PRINT_MSG("FIXME: Probably call a different function to handle mergers");
	}
	//Need to make a new chain
	else if(create)
	{
		//remember to pass a COPY of gs!
		
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
		}
		
		gs->board[tile] = newType;
		
		//Figure out if any of the adjacent tiles need to also be
		//changed- eg, non-chain tiles newly connected to the chain
		//Could have more than the immediately-adjacent tiles in
		//a corner case- when the game starts, there might be non-
		//chain clumps of 2 or more!
		
		//Dedicated function to globally clean the board?
	}
	
	PRINT_MSG("FIXME: Need to update the board!");
	
	//FIXME
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
	
	return err_code;
}
