#include <string.h>

#include "chain.h"
#include "board.h"
#include "config.h"
#include "state.h"
#include "util.h"
#include "player.h"

#ifdef TURN_DELAY
#include <unistd.h>
#endif

//FIXME: Should probably have a better way of doing this
#include "ai-random.h"


static void initializeGameState(GameState_t* gs)
{
	CHECK_NULL_PTR_FATAL(gs, "gs");
	
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

static HALE_status_t configurePlayers(GameState_t* gs, uint8_t numPlayers)
{
	//This should only be called from runGame(), so any failure in input
	//should be fatal
	CHECK_NULL_PTR_FATAL(gs, "gs");
	
	HALE_status_t err_code = HALE_OK;
	
	if(numPlayers > MAX_PLAYERS)
	{
		PRINT_MSG_INT("Too many players (%d) requested!", numPlayers);
		HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
	}
	
	//Need to record the number of players in the game into the actual state
	gs->numPlayers = numPlayers;
	
	//FIXME: Shim to allow us to continue...
	PRINT_MSG("FIXME: Need real implementation; setting all random players as a shim");
	
	int i;
	for(i = 0; i < numPlayers; i++)
	{
		gs->players[i].actions = randomActions;
	}
	
	return err_code;
	
	//FIXME: End shim
	
	//FIXME
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);
	
	return err_code;
}

//Deals <HAND_SIZE> tiles to all players
static HALE_status_t dealInitialTiles(GameState_t* gs)
{
	//This should only be called from runGame(), so any failure in input
	//should be fatal
	CHECK_NULL_PTR_FATAL(gs, "gs");
	
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
static HALE_status_t placeInitialTiles(GameState_t* gs)
{
	//This should only be called from runGame(), so any failure in input
	//should be fatal
	CHECK_NULL_PTR_FATAL(gs, "gs");
	
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


HALE_status_t makeSanitizedGameStateCopy(GameState_t* newgs, GameState_t* gs, uint8_t playerNum)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(newgs, "newgs");
	
	//Check if playerNum is in bounds
#ifndef GO_FAST_AND_BREAK_THINGS
	if(playerNum >= gs->numPlayers)
	{
		PRINT_MSG_INT("playerNum is invalid", playerNum);
		return HALE_OOB;
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
		//Don't zero out the player's own data
		if(i != playerNum)
		{
			//Other players don't get to see what tiles the other players
			//have, nor what actions the other players have registered
			memset(newgs->players[i].tiles, 0, sizeof(newgs->players[i].tiles[0])*HAND_SIZE);
			memset(&(newgs->players[i].actions), 0, sizeof(newgs->players[i].actions));
		}
	}
	
	return HALE_OK;
}


//Handles getting a tile from the player, and validating that it's valid
//for play
//Returns the tile to be played
static uint8_t getTileToPlay(GameState_t* gs)
{
	//This should only be called from runGame(), so this should NEVER
	//happen. Thus being a fatal error.
	CHECK_NULL_PTR_FATAL(gs, "gs");
	
	//Check if gs->currentPlayer is in bounds
#ifndef GO_FAST_AND_BREAK_THINGS
	if(gs->currentPlayer >= gs->numPlayers)
	{
		PRINT_MSG_INT("gs->currentPlayer is invalid", gs->currentPlayer);
		HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
	}
#endif //GO_FAST_AND_BREAK_THINGS

	//Shorthand
	uint8_t currentPlayer = gs->currentPlayer;

	//Before doing anything involving talking to players, make a sanitized
	//copy of the game state
	GameState_t gsSanitized;
	PRINT_MSG("FIXME: Check if making sanitized copy worked!");
	makeSanitizedGameStateCopy(&gsSanitized, gs, currentPlayer);

	//First, request a tile to be played from the current player
	uint8_t tile = gs->players[currentPlayer].actions.playTile(&gsSanitized, currentPlayer);
	
	//Validate that the tile is legal:
	//-player had the tile in their hand
	//-tile is a legal move on the board
	uint8_t tileInHand = 0;
	int i;
	//Check if it's in their hand
	for(i = 0; i < HAND_SIZE; i++)
	{
		if(gs->players[currentPlayer].tiles[i] == tile)
		{
			tileInHand = 1;
		}
	}
	
	//If it's not in their hand, OR it's an invalid tile to play... too bad
	if(!isValidTilePlay(gs, tile) || !tileInHand)
	{
		PRINT_MSG_INT("Player tried to play invalid tile", tile);
		PRINT_MSG_INT("Player", currentPlayer);
		PRINT_MSG_ARG("Player name", gs->players[currentPlayer].name);
		
		
		//If the tile wasn't valid, too bad- all well-behavied players/AI
		//code should ensure that it is BEFORE sending it to the game. Pick
		//a tile from their hand for them and play that.
		for(i = 0; i < HAND_SIZE; i++)
		{
			tile = gs->players[currentPlayer].tiles[i];
			if(isValidTilePlay(gs, tile))
			{
				break;
			}
		}
		PRINT_MSG_INT("Playing a tile for them", tile);
#ifndef GO_FAST_AND_BREAK_THINGS
		if(!isValidTilePlay(gs, tile))
		{
			HANDLE_UNRECOVERABLE_ERROR(HALE_SHOULD_BE_IMPOSSIBLE);
		}
#endif //GO_FAST_AND_BREAK_THINGS
	}
	
	//Find the tile in the player's hand and remove it
	for(i = 0; i < HAND_SIZE; i++)
	{
		if(gs->players[currentPlayer].tiles[i] == tile)
		{
			gs->players[currentPlayer].tiles[i] = TILE_NULL;
		}
	} 
	
	//Should just return at this point?
	//board.c should probably have a function for playing a tile, returning
	//merger/new-chain status
	//board.c should probably also have code for handling mergers (at least
	//as far as the board is concerned) and making new chains (again, at
	//least as far as the board is concerned)
	
	//Do NOT deal a new tile- that's a different step
	
	return tile;
}


//Deals with purchasing shares
static HALE_status_t handleSharePurchasePhase(GameState_t* gs)
{
	//This should only be called from runGame(), so this should NEVER
	//happen. Thus being a fatal error.
	CHECK_NULL_PTR_FATAL(gs, "gs");
	
	//Check if gs->currentPlayer is in bounds
#ifndef GO_FAST_AND_BREAK_THINGS
	if(gs->currentPlayer >= gs->numPlayers)
	{
		PRINT_MSG_INT("gs->currentPlayer is invalid", gs->currentPlayer);
		HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
	}
#endif //GO_FAST_AND_BREAK_THINGS

	HALE_status_t err_code = HALE_OK;

	uint8_t currentPlayer = gs->currentPlayer;

	GameState_t gsSanitized;
	PRINT_MSG("FIXME: Check if making sanitized copy worked!");
	makeSanitizedGameStateCopy(&gsSanitized, gs, currentPlayer);

	//Allow player to request a purchase
	uint8_t buyRequest[NUM_CHAINS] = {0};
	gs->players[currentPlayer].actions.buyStock(&gsSanitized, currentPlayer, buyRequest);
	
	//Validate that the purchase is valid:
	//-the request is for no more than 3 shares (==SHARES_PER_TURN) total
	//-the chains exist
	//-there are enough shares available to fulfill the request
	//-the player has enough money
	
	uint8_t requestIsValid = 1;
	
	
	//Ensure request is for no more than 3 (==SHARES_PER_TURN) shares total
	int i;
	int totalSharesRequested = 0;
	for(i = 0; i < NUM_CHAINS; i++)
	{
		totalSharesRequested += buyRequest[i];
	}
	if(totalSharesRequested > SHARES_PER_TURN)
	{
		PRINT_MSG_INT("Too many shares requested", totalSharesRequested);
		requestIsValid = 0;
	}
	
	
	//Ensure all requested shares have existing chains associated with them
	uint8_t chainSizes[NUM_CHAINS];
	PRINT_MSG("FIXME: check return of getChainSizes");
	getChainSizes(gs, chainSizes);
	for(i = 0; i < NUM_CHAINS; i++)
	{
		if(buyRequest[i] && chainSizes[i] < 2)
		{
			PRINT_MSG_INT("Chain doesn't exist", i);
			requestIsValid = 0;
		}
	}
	
	
	//Ensure there are enough shares to fulfill the request
	for(i = 0; i < NUM_CHAINS; i++)
	{
		if(buyRequest[i] > gs->remainingStocks[i])
		{
			PRINT_MSG_INT("Not enough shares remining of chain", i);
			requestIsValid = 0;
		}
	}
	
	
	//Ensure player has enough money to pay for the requested purchase
	int totalCost = 0;
	int32_t pricePerShare[NUM_CHAINS];
	PRINT_MSG("FIXME: check return of getChainPricesPerShare");
	PRINT_MSG("FIXME: Should probably just make one call to getChainPricesPerShare and remove the explicit call to getChainSizes");
	getChainPricesPerShare(gs, pricePerShare, NULL);
	for(i = 0; i < NUM_CHAINS; i++)
	{
		totalCost += pricePerShare[i] * buyRequest[i];
	}
	
	if(totalCost > gs->players[currentPlayer].cash)
	{
		PRINT_MSG_INT("Purchase request cost exceeds player funds", totalCost);
		requestIsValid = 0;
	}
	
	
	//If the request is valid, execute the trades
	if(requestIsValid)
	{
		gs->players[currentPlayer].cash -= totalCost;
		PRINT_MSG("FIXME: Should probably have a generic function for modifying shares a player has, handling moving them between the pool and the player...");
		for(i = 0; i < NUM_CHAINS; i++)
		{
			gs->players[currentPlayer].stocks[i] += buyRequest[i];
			gs->remainingStocks[i] -= buyRequest[i];
		}
	}
	
	//Otherwise... well, too bad. The penalty for not checking the validity
	//of your trade ahead of time is that you don't get to trade
	else
	{
		PRINT_MSG_INT("Invalid trade request; ignoring trade phase. Player", currentPlayer);
	}
	
	return err_code;
}


static HALE_status_t handleEndGameQueryPhase(GameState_t* gs, uint8_t* endGame)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(endGame, "endGame");
	
	GameState_t gsSanitized;
	PRINT_MSG("FIXME: Check if making sanitized copy worked!");
	makeSanitizedGameStateCopy(&gsSanitized, gs, gs->currentPlayer);
	
	uint8_t wantToEndGame = gs->players[gs->currentPlayer].actions.endGame(&gsSanitized, gs->currentPlayer);
	
	//Could/should probably just assign this directly, but want to keep all player actions completely
	//separate from game
	if(wantToEndGame)
	{
		*endGame = 1;
	}
	
	return HALE_OK;
}


void runGame(uint8_t numPlayers)
{
	//int i;
	HALE_status_t err_code = HALE_OK;
	
	//Keep the game state internal to the function; don't want to let
	//anything else screw with it (except for all the stuff we pass it to)
	GameState_t gs;
	//HALE_status_t err_code = HALE_OK;
	
	//Zero out the board
	initializeGameState(&gs);
	
	//Configure players (populate actions)
	configurePlayers(&gs, numPlayers);
	
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
		printf("\n\n");
		
		//Make sure the current player can actually play... If not, try
		//to do something about it.
		uint8_t numValidTiles = getNumValidTiles(&gs, gs.currentPlayer);
		while(numValidTiles == 0)
		{
			PRINT_MSG_INT("Player has no valid tiles, redealing. Player", gs.currentPlayer);
			
			//Don't try to redeal tiles if there aren't any left
			if(gs.remainingTiles > 0)
			{
				HALE_status_t err = redealTiles(&gs, gs.currentPlayer);
				
				//Running out of tiles is acceptable (could have partially redealt)
				//Anything else... not so much
				if(err != HALE_OK && err != HALE_TILE_POOL_EMPTY)
				{
					PRINT_MSG("FATAL: Couldn't handle redealing player hand");
					HANDLE_UNRECOVERABLE_ERROR(err);
				}
			}
			//If the player has no valid tiles AND there are no tiles to redeal with... tough. No playing for you!
			else
			{
				PRINT_MSG_INT("No tiles left to redeal ot player", gs.currentPlayer);
				break;
			}
			
			//Hopefully have at least one valid tile now...
			numValidTiles = getNumValidTiles(&gs, gs.currentPlayer);
		}
		
		//Request a tile to play from the current player
		//If the player has no valid tiles at this point, it must mean the
		//draw pile is out as well, and they have to skip playing
		if(numValidTiles > 0)
		{
			uint8_t tile = getTileToPlay(&gs);
			PRINT_MSG_INT("Playing tile", tile);
			
			
			//Play the tile; process any new chains/mergers
			err_code = playTile(&gs, tile, gs.currentPlayer);
			if(err_code != HALE_OK)
			{
				PRINT_MSG("FATAL: playTile failed; aborting");
				HANDLE_UNRECOVERABLE_ERROR(err_code);
			}
		}
		//If we can't play, at least make a note of it
		else
		{
			PRINT_MSG_INT("No tiles left, and player can't play; skipping tile play phase", gs.currentPlayer);
#ifdef ENABLE_PARANOID_CHECKS
			//This should never happen- should only be possible to end up with
			//an unplayable hand if the tile draw stack is empty
			if(gs.remainingTiles > 0)
			{
				PRINT_MSG("FATAL: Player can't play, yet tiles remain in draw pile");
				HANDLE_UNRECOVERABLE_ERROR(HALE_SHOULD_BE_IMPOSSIBLE);
			}
		}
#endif
		
		
		//Allow player to purchase shares
		err_code = handleSharePurchasePhase(&gs);
		if(err_code != HALE_OK)
		{
			PRINT_MSG("FATAL: handleSharePurchasePhase failed; aborting");
			HANDLE_UNRECOVERABLE_ERROR(err_code);
		}
		
		
		if(canEndGame(&gs))
		{
			PRINT_MSG("Ending game is allowed right now!");
			err_code = handleEndGameQueryPhase(&gs, &gameOver);
			if(err_code != HALE_OK)
			{
				PRINT_MSG("FATAL: handleEndGameQueryPhase failed; aborting");
				HANDLE_UNRECOVERABLE_ERROR(err_code);
			}
			
			//No need to deal tiles or set next player if we're ending the game
			if(gameOver)
			{
				break;
			}
		}
		
		
		//Deal a tile to the player that just went
		dealTile(&gs, gs.currentPlayer);
		
		//Set the next player
		gs.currentPlayer = (gs.currentPlayer + 1) % gs.numPlayers;
		
		//FIXME: Testing
		printGameBoard(&gs);
		for(int i = 0; i < gs.numPlayers; i++)
		{
			printPlayer(&gs, i);
		}
		//return;
		
#ifdef TURN_DELAY
		usleep(TURN_DELAY*1000);
#endif
		
	}
	
	//Now that the game is over, display relevant statistics:
	//-winner
	//-holdings of each player (each stock type, cash)
	//-total value of each player (bonuses + holdings + cash)
	//-Player type/name
	PRINT_MSG("FIXME: Should display a game postmortum here");
	printGameBoard(&gs);
}
