#include <stdio.h>
#include <string.h>

#include "ai-human.h"
#include "chain.h"
#include "util.h"


PlayerActions_t humanActions = {	.playTile = humanPlayTile,
					.formChain = humanFormChain,
					.mergerSurvivor = humanMergerSurvivor,
					.mergerOrder = humanMergerOrder,
					.buyStock = humanBuyStock,
					.mergerTrade = humanMergerTrade,
					.endGame = humanEndGame
				};


uint8_t humanPlayTile(GameState_t* gs, uint8_t playerNum)
{
	Player_t* p = &(gs->players[playerNum]);
	uint8_t toPlay = TILE_NULL;
	
	//FIXME: Print out stock ownerships for each player!
	
	printf("Enter a tile to play: ");
	for(int i = 0; i < HAND_SIZE; i++)
	{
		//Don't offer tiles that don't exist
		if(p->tiles[i] == TILE_NULL)
		{
			continue;
		}
		printf("%d ", p->tiles[i]);
	}
	printf("\r\nTile: ");
	
	int tmp;
	while(!scanf("%d", &tmp))
	{
		printf("Invalid entry.\r\nTile: ");
	}
	
	toPlay = tmp;
	
	//FIXME: Number entered is guaranteed to be valid here, but the TILE is not
	
	//FIXME: Somehow handle player not having any tiles to play?
	
	return toPlay;
}


chain_t humanFormChain(GameState_t* gs, uint8_t playerNum)
{
	uint8_t chainSizes[NUM_CHAINS];
	getChainSizes(gs, chainSizes);
	
	//FIXME: Print out stock ownerships for each player!
	
	printf("Enter a chain to form: ");
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		//Don't offer already-existing chains as options
		if(chainSizes[i])
		{
			continue;
		}
#ifdef ENABLE_COLOR_BOARD_PRINT
		printf("%d:%s%s%s ", i, chainConsoleColors[i], chainNames[i], ANSI_COLOR_RESET);
#else
		printf("%d:%s ", i, chainNames[i]);
#endif
	}
	printf("\r\nChain: ");
	int c;
	while(!scanf("%d", &c))
	{
		printf("Invalid entry.\r\nChain: ");
	}
	
	//FIXME: Number entered is guaranteed to be valid here, but the CHAIN is not
	
	return (chain_t)c;
}


chain_t humanMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options)
{
	printf("Select merger survivor: ");
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		//Don't offer already-existing chains as options
		if(!options[i])
		{
			continue;
		}
#ifdef ENABLE_COLOR_BOARD_PRINT
		printf("%d:%s%s%s ", i, chainConsoleColors[i], chainNames[i], ANSI_COLOR_RESET);
#else
		printf("%d:%s ", i, chainNames[i]);
#endif
	}
	printf("\r\nSurvivor: ");
	int c;
	while(!scanf("%d", &c))
	{
		printf("Invalid entry.\r\nSurvivor: ");
	}
	
	//FIXME: Number entered is guaranteed to be valid here, but the CHAIN is not
	
	return (chain_t)c;
}


void humanMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options)
{
	//FIXME: IMPLEMENT!
	PRINT_MSG("NOT IMPLEMENTED");
}


void humanBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy)
{
	//Zero out toBuy array, to be safe
	memset(toBuy, 0, NUM_CHAINS);
	
	int32_t pps[NUM_CHAINS];
	getChainPricesPerShare(gs, pps, NULL);
	
	//List options
	printf("Chain\tPPS\tShares\r\n");
	for(int i = 0; i < NUM_CHAINS; i++)
	{
#ifdef ENABLE_COLOR_BOARD_PRINT
		printf("%d:%s%s%s\t", i, chainConsoleColors[i], chainNames[i], ANSI_COLOR_RESET);
#else
		printf("%d:%s\t", i, chainNames[i]);
#endif
		printf("$%d\t%d\r\n", pps[i], gs->remainingStocks[i]);
	}
	
	printf("Enter indices of stocks to buy, up to 3 ($%d available): ", gs->players[playerNum].cash);
	
	//Request input
	int purchase[SHARES_PER_TURN];
	
	//Just take what you get... bad input is bad input
	scanf("%d %d %d", &(purchase[0]), &(purchase[1]), &(purchase[2]));
	
	//Populate output
	for(int i = 0; i < SHARES_PER_TURN; i++)
	{
		//Only do basic checks to ensure in bounds
		if(purchase[i] < NUM_CHAINS)
		{
			toBuy[purchase[i]]++;
		}
	}
	
	return;
}


void humanMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell)
{
	int32_t pps[NUM_CHAINS];
	uint8_t chainSizes[NUM_CHAINS];
	getChainPricesPerShare(gs, pps, chainSizes);
	
	//FIXME: Print out some salient information... like which chains are merging
#ifdef ENABLE_COLOR_BOARD_PRINT
	printf("%d:%s%s%s is absorbing %d:%s%s%s\r\n", survivor, chainConsoleColors[survivor], chainNames[survivor], ANSI_COLOR_RESET, defunct, chainConsoleColors[defunct], chainNames[defunct], ANSI_COLOR_RESET);
	printf("Remaining shares of %d:%s%s%s: %d\r\n", survivor, chainConsoleColors[survivor], chainNames[survivor], ANSI_COLOR_RESET, gs->remainingStocks[survivor]);
#else
	printf("%d:%s is absorbing %d:%s", survivor, chainNames[survivor], defunct, chainNames[defunct]);
	printf("Remaining shares of %d:%s: %d\r\n", survivor, chainNames[survivor], gs->remainingStocks[survivor]);
#endif
	
	//Nope, error checking is for chumps. Humans can verify their own inputs
	printf("Enter numer of shares to trade FOR and to sell: ");
	int tmptrade, tmpsell;
	scanf("%d %d", tmptrade, tmpsell);
	*tradeFor = tmptrade;
	*sell = tmpsell;
	
	//Segfaulting... see if it's before or after here
	printf("DONE\r\n");
	
	return;
}


uint8_t humanEndGame(GameState_t* gs, uint8_t playerNum)
{
	//FIXME: IMPLEMENT!
	PRINT_MSG("NOT IMPLEMENTED");
	return 1;
}
