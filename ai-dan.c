
#include "ai-dan.h"
#include "config.h"
#include "util.h"
#include "board.h"

const PlayerActions_t danActions = {	.playTile = danPlayTile,
					.formChain = danFormChain,
					.mergerSurvivor = danMergerSurvivor,
					.mergerOrder = danMergerOrder,
					.buyStock = danBuyStock,
					.mergerTrade = danMergerTrade,
					.endGame = danEndGame,
					.typeName = "aidan"
				};


uint8_t danPlayTile(GameState_t* gs, uint8_t playerNum)
{
	//Thigs to consider:
	//-Want to expand chains owned by you
	//-Want to NOT expand chains NOT owned by you (or MORE owned by others)
	//-Want to form chains
	//-Want to cause mergers where you benefit
	
	
	//Form a new chain (that you already own stock in)		50
	//Cause a highly-beneficial merger (majority holder)		40
	//Form a new chain nobody owns stock in				30
	//Expand a chain you're majority shareholder			20
	//
	//Play a touch-nothing tile					10
	//Expand a chain you're minority shareholder			0
	//
	//Form a chain somebody else has stock in			-10
	//Expand somebody else's chain (no majority/minority holding)	-20
	//Cause a merger that benefits others				-30
	
	//Get chain sizes... we'll probably need those?
	uint8_t chainSizes[NUM_CHAINS];
	getChainSizes(gs, chainSizes);
	
	//Make the code a LITTLE less verbose
	Player_t* p = &(gs->players[playerNum]);
	
	//Figure out who owns what
	uint8_t playerStocks[MAX_PLAYERS][NUM_CHAINS];
	for(int i = 0; i < gs->numPlayers; i++)
	{
		memcpy(playerStocks[i], gs->players[i].stocks, NUM_CHAINS);
	}
	
	
	uint8_t bestTile = TILE_NULL;
	int bestValue = -100;
	
	for(int i = 0; i < HAND_SIZE; i++)
	{
		uint8_t tile = p->tiles[i];
		//No need to investigate invalid moves
		if(!isValidTilePlay(gs, tile))
		{
			continue;
		}
		
		int val = 0;
		
		//Figure out what's around this tile
		chain_t adjacentTiles[4];
		getAdjacentSquares(gs, tile, adjacentTiles);
		
		
		//FIXME: Check if it would expand a chain
		//FIXME: Check if it would cause a merger
		//FIXME: Check if it would create a new chain (NOTE: Must be valid to do so, since it's a valid tile)
		uint8_t wouldFormChain = 0;
		uint8_t wouldCauseMerger = 0;
		uint8_t wouldExpandChain = 0;
		chain_t expandChain = CHAIN_NONE;
		chain_t causeMerger[4] = {CHAIN_NONE, CHAIN_NONE, CHAIN_NONE, CHAIN_NONE};
		uint8_t adjacentChainMask[NUM_CHAINS];
		memset(adjacentChainMask, 0, NUM_CHAINS);
		uint8_t numAdjacentChains = 0;
		
		for(int j = 0; j < 4; j++)
		{
			if(adjacentTiles[j] != CHAIN_EMPTY)
			{
				if(adjacentTiles[j] == CHAIN_NONE)
				{
					wouldFormChain = 1;
					continue;
				}
				else if(!adjacentChainMask[adjacentTiles[j]])
				{
					adjacentChainMask[adjacentTiles[j]] = 1;
					causeMerger[numAdjacentChains] = adjacentTiles[j];
					expandChain = adjacentTiles[j];
					numAdjacentChains++;
				}
			}
		}
		
		//Clean up wouldFormChain
		wouldFormChain = (wouldFormChain && !numAdjacentChains);
		
		//Check if we'd cause a merger or expand a chain
		if(numAdjacentChains > 1)
		{
			wouldCauseMerger = 1;
		}
		else if(numAdjacentChains == 1)
		{
			wouldExpandChain = 1;
		}
		
		
		//FIXME: Do stuff here
		
		//Basically, try not to expand chains unless you've got a large stake in them
		if(wouldExpandChain)
		{
			//Check if this expansion is "good" and weight accordingly
			
			//First, figure out who is majority and minority
			uint8_t maxHolding = 0;
			uint8_t minHolding = 0;
			
			for(int j = 0; j < gs->numPlayers; j++)
			{
				if(playerStocks[j][expandChain] > maxHolding)
				{
					minHolding = maxHolding;
					maxHolding = playerStocks[j][expandChain];
				}
				else if(playerStocks[j][expandChain] > minHolding)
				{
					minHolding = playerStocks[j][expandChain];
				}
			}
			
			if(p->stocks[expandChain] == maxHolding)
			{
				val = 20;
			}
			else if(p->stocks[expandChain] == minHolding)
			{
				val = 0;
			}
			else
			{
				val = -20;
			}
		}
		else if(wouldCauseMerger)
		{
			//FIXME
			//Cause a highly-beneficial merger (majority holder)		40
			//Cause a merger that benefits others				-30
			//FIXME: Cause an OK merger (minority holder)?
			
			//Remember that more than two chains may be merging!
			//FIXME: Probably want to weight things based on available cash, size of surviving chain, etc.
			
			
			
		}
		else if(wouldFormChain)
		{
			//This is usually good, but may want to account
			//for one or more players having stock in defunct
			//chain(s)
			
			//Form a new chain (that you already own stock in)		50
			//Form a new chain nobody owns stock in				30
			//Form a chain somebody else has stock in			-10
			
			for(int j = 0; j < NUM_CHAINS; j++)
			{
				//If it's not a defunt chain, ignore
				if(chainSizes[j] != 0)
				{
					continue;
				}
				
				//For defunct chains...
				//REALLY good if it's a chain we have stock in (more than other people)
				//GOOD if NOBODY has stock in it
				//Not so good if somebody else has stock (or at least more than you)
				
				//First, figure out who is majority and minority
				//NOTE: This is duplicated elsewhere; should probably not do that
				uint8_t maxHolding = 0;
				uint8_t minHolding = 0;
				
				for(int k = 0; k < gs->numPlayers; k++)
				{
					if(playerStocks[k][j] > maxHolding)
					{
						minHolding = maxHolding;
						maxHolding = playerStocks[k][j];
					}
					else if(playerStocks[k][j] > minHolding)
					{
						minHolding = playerStocks[k][j];
					}
				}
				
				//So if we found found a defunct chain we're majority holder in... great!
				//Don't bother looking further, just set value and break
				if(p->stocks[j] == maxHolding)
				{
					val = 50;
					break;
				}
				//If NOBODY owns shares, value is at LEAST 30- set it
				//...but keep going in case we find a better option
				else if(maxHolding == 0)
				{
					val = 30;
				}
				//FIXME: Kind of a lousy criteria, but let's do it anyway
				//Should really better account for DIFFERENCES in holding (eg, off by 1 isn't as bad as off by 20)
				else if(p->stocks[j] <= minHolding && minHolding)
				{
					val = val > -10 ? val : -10;
				}
			}
		}
		
		
		//If this move is deemed better than previous moves, note it
		if(val > bestValue)
		{
			bestValue = val;
			bestTile = tile;
		}
		
	}
	
	return bestTile;
}


chain_t danFormChain(GameState_t* gs, uint8_t playerNum)
{
	//Avoid forming chains others have stock in (unless we have more)
	//Otherwise, just try to form expensive chains?
	//Implementing this is meaningless until we adjust merger trading functionality
	PRINT_MSG("FIXME: Incomplete");
	return CHAIN_NONE;
}


chain_t danMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options)
{
	//FIXME: Incomplete
	PRINT_MSG("FIXME: Incomplete");
	return CHAIN_NONE;
}


void danMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options)
{
	//FIXME: Incomplete
	PRINT_MSG("FIXME: Incomplete");
}


void danBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy)
{
	//Iterates through every possible purchase, picks the best for
	//for RIGHT NOW
	
	int32_t bestValue = 0;
	uint8_t buy[3] = {CHAIN_EMPTY, CHAIN_EMPTY, CHAIN_EMPTY};
	
	int32_t sharePrices[NUM_CHAINS];
	uint8_t chainSizes[NUM_CHAINS];
	
	Player_t* p = &(gs->players[playerNum]);
	
	getChainPricesPerShare(gs, sharePrices, chainSizes);
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		//If there are no stocks remaining of that type, or we can't
		//afford a share, OR the chain doesn't exist, don't bother
		//looking further
		if( (chainSizes[i] < 2) || (gs->remainingStocks[i] == 0) || (sharePrices[i] > p->cash) )
		{
			continue;
		}
		//Otherwise, "buy" a share
		p->cash -= sharePrices[i];
		p->stocks[i]++;
		gs->remainingStocks[i]--;
		
		for(int j = 0; j < NUM_CHAINS; j++)
		{
			//Need inner loops to execute, so add a method for
			//skipping simulating a buy
			uint8_t skip1 = ((chainSizes[j] < 2) || (gs->remainingStocks[j] == 0) || (sharePrices[j] > p->cash));
			if(skip1)
			{
				//continue;
			}
			else
			{
				p->cash -= sharePrices[j];
				p->stocks[j]++;
				gs->remainingStocks[j]--;
			}
			
			for(int k = 0; k < NUM_CHAINS; k++)
			{
				uint8_t skip2 = ((chainSizes[k] < 2) || (gs->remainingStocks[k] == 0) || (sharePrices[k] > p->cash));
				if(skip2)
				{
					//continue;
				}
				else
				{
					p->cash -= sharePrices[k];
					p->stocks[k]++;
					gs->remainingStocks[k]--;
				}
				
				//Calculate value of this purchase, record if it's the best
				int32_t val;
				calculatePlayerValue(gs, playerNum, &val);
				//Use >= to encourage actually buying stocks
				if(val >= bestValue)
				{
					bestValue = val;
					buy[0] = i;
					buy[1] = skip1 ? CHAIN_EMPTY : j;
					buy[2] = skip2 ? CHAIN_EMPTY : k;
				}
				
				if(!skip2)
				{
					p->cash += sharePrices[k];
					p->stocks[k]--;
					gs->remainingStocks[k]++;
				}
			}//k
			
			if(!skip1)
			{
				p->cash += sharePrices[j];
				p->stocks[j]--;
				gs->remainingStocks[j]++;
			}
		}//j
		
		//"return" the share
		p->cash += sharePrices[i];
		p->stocks[i]--;
		gs->remainingStocks[i]++;
	}//i
	
	
	//Should now have the best value- format purchase order
	memset(toBuy, 0x00, NUM_CHAINS);
	for(int i = 0; i < 3; i++)
	{
		if(buy[i] < CHAIN_NONE)
		{
			toBuy[buy[i]]++;
		}
	}
}


void danMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell)
{
	//FIXME: Want to change this- some kind of heuristic for the value
	//of keeping shares in the defunct chain, based on odds it may reappear
	
	int32_t sharePrices[NUM_CHAINS];
	uint8_t chainSizes[NUM_CHAINS];
	
	uint8_t bestTrade = 0;
	
	//If we only have one share, the loop won't run- greedy option is
	//to sell here, so make that the default
	uint8_t bestSell = gs->players[playerNum].stocks[defunct];
	uint8_t bestValue = 0;
	
	Player_t* p = &(gs->players[playerNum]);
	
	getChainPricesPerShare(gs, sharePrices, chainSizes);
	
	//Need to adjust the board- eliminate the defunct chain, turn it into the suriving chain
	//Otherwise value proposition is off
	//FIXME: This isn't perfect- final size of the surviving chain will be at least one bigger than we think
	for(int i = 0; i < BOARD_TILES; i++)
	{
		if(gs->board[i] == defunct)
		{
			gs->board[i] = survivor;
		}
	}
	
	//Trade on outside loop, so never end up testing an always-wrong
	//"keep one share of defunct chain" option
	for(int i = 0; i < p->stocks[defunct]/2; i++)
	{
		//If there aren't enough shares to trade for, don't bother
		//evaluating this option
		if(i > gs->remainingStocks[survivor])
		{
			break;
		}
		
		//"trade"
		p->stocks[defunct] -= 2*i;
		p->stocks[survivor] += i;
		//No need to deal with remaining stocks- we've done the
		//check we care about on them
		
		
		//We really don't need a loop here- just sell everything
		//we don't trade. Keeping defunct stocks is believed to
		//be dumb by this AI
		int j = p->stocks[defunct]; //p->stocks[defunct] has already been reduced by "trading"
		//"sell"
		p->stocks[defunct] -= j;
		p->cash += j*sharePrices[defunct];
		
		//Calculate value of this option
		int32_t val;
		calculatePlayerValue(gs, playerNum, &val);
		if(val > bestValue)
		{
			bestValue = val;
			bestTrade = i;
			bestSell = j;
		}
		
		//undo "sell"
		p->stocks[defunct] += j;
		p->cash -= j*sharePrices[defunct];
		
		
		//undo the "trade"
		p->stocks[defunct] += 2*i;
		p->stocks[survivor] -= i;
	}
	
	//Place trade request, now that the best has been found
	*tradeFor = bestTrade;
	*sell = bestSell;
}


//Greedy behavior- no real reason to change this
uint8_t danEndGame(GameState_t* gs, uint8_t playerNum)
{
	//If we're winning, end the game. Otherwise, don't.
	int32_t maxPlayerValue = 0;
	int32_t ourValue = 0;
	
	calculatePlayerValue(gs, playerNum, &ourValue);
	
	for(int i = 0; i < gs->numPlayers; i++)
	{
		//Don't count ourselves- save some cycles
		if(i == playerNum)
		{
			continue;
		}
		
		int32_t pv = 0;
		calculatePlayerValue(gs, i, &pv);
		
		if(pv > maxPlayerValue)
		{
			maxPlayerValue = pv;
		}
	}
	
	//Return 0 if we're not winning, 1 if we are
	//No, a tie is NOT good enough.
	return (ourValue > maxPlayerValue);
}
