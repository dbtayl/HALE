#ifndef HALE_PLAYER_H
#define HALE_PLAYER_H

#include "config.h"
#include "state.h"

//Explanation of required functions for a player
//typedef struct
//{
	//uint8_t (*playTile)(GameState_t* gs, uint8_t playerNum);
	//chain_t (*formChain)(GameState_t* gs, uint8_t playerNum);
	//chain_t (*mergerSurvivor)(GameState_t* gs, uint8_t playerNum, uint8_t* options);
	//void (*mergerOrder)(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);
	//void (*buyStock)(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);
	//void (*mergerTrade)(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);
	//uint8_t (*endGame)(GameState_t* gs, uint8_t playerNum);
	//char* name;
//} PlayerActions_t;



/* uint8_t (*playTile)(GameState_t* gs, uint8_t playerNum);
 * 
 * Called when given the option to play a tile. Should return a tile 
 * (0-(BOARD_TILES-1)) from the player's hand that's valid to play.
 */


/* chain_t (*formChain)(GameState_t* gs, uint8_t playerNum);
 * 
 * Called when the player plays a tile that would result in them forming a
 * chain, in order to select which chain to form. Should return a valid
 * chain < CHAIN_NONE (ie, a real chain that doesn't already exist.
 */


/* chain_t (*mergerSurvivor)(GameState_t* gs, uint8_t playerNum, uint8_t* options);
 * 
 * Called when the player is required to select one of two+ equal-sized chains
 * to become the surviving chain in a merger. <options> will contain a one-hot
 * list of chains that may be selected from (NUM_CHAINS size array). One
 * of those (indices) must be returned.
 */
 
 /* void (*mergerOrder)(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);
  * 
  * Called in the event that a merger involves 3 or more chains, and the
  * player needs to decide which order to merge them in. <survivor> will
  * already be specified, and <options> will contain a default ordering.
  * Chains not involved in the merger will be assigned a value of 0xFF; the
  * involved chains will be numbered 0-(n-1). This variable should be manipulated
  * to number those chains, with the lowest number being merged first.
  * 
  * Chains not to be merged- including the surviving chain- MUST have the
  * value 0xFF.
  */


/* void (*buyStock)(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);
 * 
 * Called when the player is given the opportunity to buy stock. The player
 * should write the numbers of each kind of stock they want to buy into the
 * array <toBuy>, with the TYPE of stock being selected by the index into
 * the array, using CHAIN_* values.
 * The number of stocks purchased must be <= 3 (==SHARES_PER_TURN), and the player must have
 * sufficient cash to purchase the shares.
 */


/* void (*mergerTrade)(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);
 * 
 * Called when the player holds stock in a chain that is disappearing as
 * the result of a merger. This function should populate <tradeFor> and
 * <sell> with the number of shares of <survivor> it wants to trade for,
 * and the number of shares of <sell> it wants to sell.
 * 2*<tradeFor> + <sell> <= <gs->players[playerNum].stocks[defunct]>
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


//Prints out entire player state
void printPlayer(GameState_t* gs, uint8_t playerNum);

#endif
