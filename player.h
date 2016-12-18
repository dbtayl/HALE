#ifndef HALE_PLAYER_H
#define HALE_PLAYER_H

#include "config.h"
#include "state.h"

//Explanation of required functions for a player
//typedef struct
//{
	//uint8_t (*playTile)(GameState_t* gs, uint8_t playerNum);
	//chain_t (*formChain)(GameState_t* gs, uint8_t playerNum);
	//chain_t (*mergerSurvivor)(GameState_t* gs, uint8_t playerNum, uint8_t numOptions, chain_t* options);
	//void (*buyStock)(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);
	//void (*mergerTrade)(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t merged, uint8_t* tradeFor, uint8_t* sell);
	//uint8_t (*endGame)(GameState_t* gs, uint8_t playerNum);
	//char* name;
//} PlayerActions_t;



/* uint8_t (*playTile)(GameState_t* gs, uint8_t playerNum);
 * 
 * Called when given the option to play a tile. Should return a tile 
 * (0-BOARD_TILES) from the player's hand that's valid to play.
 */


/* chain_t (*formChain)(GameState_t* gs, uint8_t playerNum);
 * 
 * Called when the player plays a tile that would result in them forming a
 * chain, in order to select which chain to form. Should return a valid
 * chain < CHAIN_NONE (ie, a real chain that doesn't already exist.
 */


/* chain_t (*mergerSurvivor)(GameState_t* gs, uint8_t playerNum, uint8_t numOptions, chain_t* options);
 * 
 * Called when the player is required to select one of two+ equal-sized chains
 * to become the surviving chain in a merger. Should return one of the options
 * provided in <options>
 */


/* void (*buyStock)(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);
 * 
 * Called when the player is given the opportunity to buy stock. The player
 * should write the numbers of each kind of stock they want to buy into the
 * array <toBuy>, with the TYPE of stock being selected by the index into
 * the array, using CHAIN_* values.
 * The number of stocks purchased must be <= 3, and the player must have
 * sufficient cash to purchase the shares.
 */


/* void (*mergerTrade)(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t merged, uint8_t* tradeFor, uint8_t* sell);
 * 
 * Called when the player holds stock in a chain that is disappearing as
 * the result of a merger. This function should populate <tradeFor> and
 * <sell> with the number of shares of <survivor> it wants to trade for,
 * and the number of shares of <sell> it wants to sell.
 * 2*<tradeFor> + <sell> <= <gs->players[playerNum].stocks[merged]>
 */


/* uint8_t (*endGame)(GameState_t* gs, uint8_t playerNum);
 * 
 * Called when the player has the opportunity to end the game. Returning 0
 * does not end the game, returning anything else will end the game.
 */


//Adds a tile to a player's hand. Nominally a simple function, but may
//be more complex for non-PC HALE
HALE_status_t giveTile(GameState_t* gs, uint8_t tile, uint8_t playerNum);


//Calculates the current player cash value
HALE_status_t calculatePlayerValue(GameState_t* gs, uint8_t playerNum, int32_t* val);

#endif
