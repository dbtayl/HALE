#include <string.h>

#include "chain.h"
#include "board.h"
#include "config.h"
#include "state.h"
#include "util.h"


void initializeGameState(GameState_t* gs)
{
	CHECK_NULL_PTR(gs, "gs");
	
	gs->numPlayers = 0;
	gs->currentPlayer = 0;
	
	memset(&(gs->players), 0, MAX_PLAYERS*sizeof(Player_t));
	
	//use a loop here instead of memset, since we might for some ungodly
	//reason not use a single byte for each remaining stock in the future
	int i;
	for(i = 0; i < NUM_CHAINS; i++)
	{
		gs->remainingStocks[i] = NUM_STOCKS;
	}
	
	//Ditto for each space on the board
	for(i = 0; i < BOARD_TILES; i++)
	{
		gs->board[i] = TILE_NULL;
	}
	
	//Initialize and shuffle tiles
	gs->remainingTiles = BOARD_TILES;
	for(i = 0; i < BOARD_TILES; i++)
	{
		gs->tilePool[i] = i;
	}
	shuffleTiles(gs->tilePool);
	
	//Initialize players (cash, stocks, tiles)
	for(i = 0; i < MAX_PLAYERS; i++)
	{
		gs->players[i].cash = STARTING_CASH;
		
		memset(gs->players[i].stocks, 0, sizeof(gs->players[i].stocks[0]) * NUM_CHAINS);
		
		int j;
		for(j = 0; j < HAND_SIZE; j++)
		{
			gs->players[i].tiles[j] = TILE_NULL;
		}
	}
	
	//Set the current player to something valid
	gs->currentPlayer = 0;
}

//Deals <HAND_SIZE> tiles to all players
HALE_status_t dealInitialTiles(GameState_t* gs)
{
	CHECK_NULL_PTR(gs, "gs");
	
	HALE_status_t err_code = HALE_OK;
	
	int i, j;
	for(i = 0; i < gs->numPlayers; i++)
	{
		for(j = 0; j < HAND_SIZE; j++)
		{
			err_code = dealTile(gs, i);
			//We won't accept ANY errors in initial deal
			if(err_code)
			{
				PRINT_MSG("Not accepting ANY errors during initial tile dealing");
				HANDLE_UNRECOVERABLE_ERROR(err_code);
			}
		}
	}
	
	return err_code;
}


//Draws <numPlayers> tiles and places them on the board
HALE_status_t placeInitialTiles(GameState_t* gs)
{
	CHECK_NULL_PTR(gs, "gs");
	
	HALE_status_t err_code = HALE_OK;
	int i;
	for(i = 0; i < gs->numPlayers; i++)
	{
		uint8_t tile;
		err_code = drawTile(gs, &tile);
		if(err_code != HALE_OK)
		{
			PRINT_MSG("Error drawing tiles for initial board");
			HANDLE_UNRECOVERABLE_ERROR(err_code);
		}
		
		//Normally you'd want to call a function to play a tile so it can
		//handle all of the chain creation/merging/etc. Here we literally
		//just want to put them on the board, and explicitly NOT form chains.
		//So we modify the board state directly
		gs->board[tile] = CHAIN_NONE;
	}
	
	return err_code;
}


void makeSanitizedGameStateCopy(GameState_t* gs, GameState_t* newgs, uint8_t playerNum)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(newgs, "newgs");
	
	//Check if playerNum is in bounds
#ifndef GO_FAST_AND_BREAK_THINGS
	if(playerNum >= gs->numPlayers)
	{
		PRINT_MSG_INT("playerNum is invalid", playerNum);
		HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
	}
#endif //GO_FAST_AND_BREAK_THINGS
	
	//Board state, stock states, etc. get copied over entirely. They're
	//public information
	newgs->numPlayers = gs->numPlayers;
	memcpy(newgs->board, gs->board, sizeof(gs->board[0])*BOARD_TILES);
	memcpy(newgs->remainingStocks, gs->remainingStocks, sizeof(gs->remainingStocks[0])*NUM_CHAINS);
	newgs->remainingTiles = gs->remainingTiles;
	newgs->currentPlayer = gs->currentPlayer;
	
	//Zero out the remaining tile pool- the players don't get to see that
	memset(newgs->tilePool, 0, sizeof(newgs->tilePool[0])*BOARD_TILES);
	
	//Handle player states specially- each player gets to know their own
	//state, but not the COMPLETE state of the rest of the players
	//Copy over complete states
	memcpy(newgs->players, gs->players, sizeof(gs->players[0])*gs->numPlayers);
	
	//zero out the information you don't get
	int i;
	for(i = 0; i < gs->numPlayers; i++)
	{
		//Don't zero out the players own data
		if(i == playerNum)
		{
			//Other players don't get to see what tiles the other players
			//have, nor what actions the other players have registered
			memset(newgs->players[i].tiles, 0, sizeof(newgs->players[i].tiles[0])*HAND_SIZE);
			memset(&(newgs->players[i].actions), 0, sizeof(newgs->players[i].actions));
		}
	}
}


void runGame(uint8_t numPlayers)
{
	int i;
	
	if(numPlayers > MAX_PLAYERS)
	{
		PRINT_MSG_INT("Too many players (%d) requested!", numPlayers);
		HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
	}
	
	//Keep the game state internal to the function; don't want to let
	//anything else screw with it (except for all the stuff we pass it to)
	GameState_t gs;
	//HALE_status_t err_code = HALE_OK;
	
	//Zero out the board
	initializeGameState(&gs);
	
	//Configure players (populate actions)
	gs.numPlayers = numPlayers;
	//FIXME
	for(i = 0; i < numPlayers; i++)
	{
		PRINT_MSG("FIXME: Need to populate player function structures");
		PRINT_MSG("FIXME: Each player will be able to return a name string... verify/sanitize those once here- and record them locally so they can't change it later");
	}
	
	//Deal initial tiles
	dealInitialTiles(&gs);
	
	//Randomize first player
	gs.currentPlayer = getRandom8(0, gs.numPlayers-1);
	
	//Place <numPlayers> random tiles on the board
	placeInitialTiles(&gs);
	
	//So now everybody has tiles, the board is set up... time to start!
	uint8_t gameOver = 0;
	while(!gameOver)
	{
		PRINT_MSG("FIXME: Need to run actual game!");
		
		PRINT_MSG("FIXME: Play a tile");
		PRINT_MSG("FIXME: Need to handle a player having no playable tiles- should be handled here-ish");
		PRINT_MSG("FIXME: Need to look up how permanently-unplayable tiles work- should be handled here-ish");
		
		PRINT_MSG("FIXME: Handle any new chains/mergers");
		PRINT_MSG("FIXME: Allow buying of shares");
		PRINT_MSG("FIXME: Allow option to end game, if applicable");
		
		
		//Deal a tile to the player that just went
		dealTile(&gs, gs.currentPlayer);
		
		//Set the next player
		gs.currentPlayer = (gs.currentPlayer + 1) % gs.numPlayers;
	}
	
	//Now that the game is over, display relevant statistics:
	//-winner
	//-holdings of each player (each stock type, cash)
	//-total value of each player (bonuses + holdings + cash)
	//-Player type/name
	PRINT_MSG("FIXME: Should display a game postmortum here");
}
