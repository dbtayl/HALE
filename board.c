#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "board.h"
#include "player.h"
#include "util.h"


//Not a particularly smart algorithm; for each tile index, picks a random
//different tile index and swaps the two
void shuffleTiles(uint8_t* tiles)
{
    CHECK_NULL_PTR(tiles, "tiles");
    
	int i;
	for(i = 0; i < BOARD_TILES; i++)
	{
		int j = rand() % BOARD_TILES;
		uint8_t tmp = tiles[i];
		tiles[i] = tiles[j];
		tiles[j] = tmp;
	}
	return;
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
		HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
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


uint8_t isValidTilePlay(GameState_t* gs, uint8_t tile)
{
    CHECK_NULL_PTR(gs, "gs");
    
    
    
    //FIXME: Replace this with various calls to "wouldCauseMerger" and
    //"wouldCreateChain"
    
    
    
    
	//Invalid if:
	//-would merge two safe chains
	//-would create a chain when there are already 7 in play
	
	//First, find out what tiles types are adjacent
	//Get indices of board adjacent to tile: Up, left, right, down
    int i;
    uint8_t indices[4] = {SQUARE_LEFT(tile), SQUARE_RIGHT(tile), SQUARE_UP(tile), SQUARE_DOWN(tile)};
    chain_t uniqueChains[4] = {CHAIN_EMPTY};
    int numUniq = 0;
    
    //Resolve to state of each square and count up how many uniqe tile types
    //immediately surround the proposed placement
    chain_t adj[4];
    for(i = 0; i < 4; i++)
    {
        uint8_t idx = indices[i];
        adj[i] = (idx == BOARD_NULL) ? CHAIN_EMPTY : gs->board[idx];
        
        //Check if the square we just looked at is unique (so far)
        int j;
        uint8_t uniq = 1;
        for(j = 0; j < i; j++)
        {
            if(adj[i] == adj[j])
            {
                uniq = 0;
            }
        }
        
		//If it was unique- and not empty-, add to the list
		if(uniq && adj[i] != CHAIN_EMPTY)
		{
			uniqueChains[numUniq] = adj[i];
			numUniq++;
		}
	}
	
	//Trivial case- all empty squares. Valid!
	if(numUniq == 0)
	{
		return (uint8_t)1;
	}
	
	//Fairly trivial case: just one thing around
	if(numUniq == 1)
	{
		//If this would create a new chain, need to check if that's OK
		if(uniqueChains[0] == CHAIN_NONE)
		{
			PRINT_MSG("FIXME: Implement checking if it's valid to form a new chain");
		}
        //If this would just expand a single chain, no problem
		else
		{
			return 1;
		}
	}
	
	PRINT_MSG("FIXME: Need to finish isValidTilePlay()");
	
	return 0;
}


uint8_t wouldCauseMerger(GameState_t* gs, uint8_t tile, uint8_t* numMergingChains, chain_t* mergingChains)
{
    //NOTE: Don't bother checking if the tile has already been played-
    //the return value should be valid regardless
    CHECK_NULL_PTR(gs, "gs");
    
    PRINT_MSG("FIXME: Implement wouldCauseMerger()");
    HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
    
    return 0;
}


uint8_t wouldCreateChain(GameState_t* gs, uint8_t tile)
{
    //NOTE: Don't bother checking if the tile has already been played-
    //the return value should be valid regardless
    CHECK_NULL_PTR(gs, "gs");
    
    if(tile >= BOARD_TILES)
    {
        PRINT_MSG_INT("<tile> is out of bounds", tile);
        HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
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
        chain_t c = gs->board[i]
        if(c < CHAIN_NONE)
        {
            sizes[c]++;
        }
    }
    
    return HALE_OK;
}
