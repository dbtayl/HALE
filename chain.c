#include <string.h>

#include "board.h"
#include "chain.h"
#include "util.h"


const char* chainNames[NUM_CHAINS+1] = {	"LUXOR",
											"TOWER",
											"WORLDWIDE",
											"AMERICAN",
											"FESTIVAL",
											"IMPERIAL",
											"CONTINENTAL",
											"NONE"};

const char* chainConsoleColors[NUM_CHAINS] = {ANSI_COLOR_RED, ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, ANSI_COLOR_GREEN, ANSI_COLOR_BLUE, ANSI_COLOR_MAGENTA, ANSI_COLOR_CYAN};


HALE_status_t getChainPricesPerShare(GameState_t* gs, int32_t* prices, uint8_t* sizesOut)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(prices, "prices");
	
	
	uint8_t sizes[NUM_CHAINS];
	getChainSizes(gs, sizes);
	
	//Cycle through the chains and calculate prices
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		if(sizes[i] < 6)
		{
			prices[i] = sizes[i]*100;
		}
		else if(sizes[i] > 40)
		{
			prices[i] = 1000;
		}
		//sizes[i] somewhere betwen 6 and 40, inclusive
		else
		{
			prices[i] = 600 + ((sizes[i]-1)/10) * 100;
		}
		
		//Add "bonus cost" for more expensive chains
		//+$100 for mid-tier
		if(i > CHAIN_TOWER)
		{
			prices[i] += 100;
		}
		//ANOTHER +$100 for top-tier
		if(i > CHAIN_FESTIVAL)
		{
			prices[i] += 100;
		}
	}
	
	//If requested (via non-NULL pointer), also return the sizes of the chains
	if(sizesOut != NULL)
	{
		memcpy(sizesOut, sizes, sizeof(sizes[0])*NUM_CHAINS);
	}
	
	return HALE_OK;
}


HALE_status_t getChainBonuses(GameState_t* gs, int32_t* prices, int32_t* majBonus, int32_t* minBonus)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(majBonus, "majBonus");
	CHECK_NULL_PTR(minBonus, "inBonus");
	
	//If we didn't get a price list in, we'll have to calculate that first
	int32_t newPrices[NUM_CHAINS];
	if(prices == NULL)
	{
		prices = newPrices;
		getChainPricesPerShare(gs, prices, NULL);
	}
	
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		majBonus[i] = 10 * prices[i];
		minBonus[i] = 5 * prices[i];
	}
	
	return HALE_OK;
}


HALE_status_t calculatePlayerBonus(GameState_t* gs, uint8_t playerNum, chain_t chain, int32_t* bonus)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(bonus, "bonus");
	
	//Convenience- dereferencing and indexing is so verbose
	uint8_t playerHoldings = gs->players[playerNum].stocks[chain];
	
	//No stock, no bonus- check this first before bothering with other
	//calculations
	if(!playerHoldings)
	{
		*bonus = 0;
		return HALE_OK;
	}
	
	//Figure out how many players hold how much stock
	uint8_t majHolders = 0;
	uint8_t minHolders = 0;
	uint8_t majHolding = 0;
	uint8_t minHolding = 0;
	for(int i = 0; i < gs->numPlayers; i++)
	{
		//cph == current player holdings
		uint8_t cph = gs->players[i].stocks[chain];
		
		//If this player is below minimum holdings, skip
		if(cph < minHolding)
		{
			continue;
		}
		//Above majority, kick previous majority down to minority and update
		else if(cph > majHolding)
		{
			minHolders = majHolders;
			minHolding = majHolding;
			majHolders = 1;
			majHolding = cph;
		}
		//At majority, add to it
		else if(cph == majHolding)
		{
			majHolders++;
		}
		//Below majority, but above previous minority- set new min
		else if(cph > minHolding)
		{
			minHolders = 1;
			minHolding = cph;
		}
		//Not above max, at max, above minority, or below minority... must be at minority
		else
		{
			minHolders++;
		}
	}
	
	//See where we fall
	//Below min- no bonus
	//At min, but two or more majority holders? No bonus
	if( (playerHoldings < minHolding) || ((playerHoldings < majHolding) && (majHolders > 1)) )
	{
		*bonus = 0;
		return HALE_OK;
	}
	
	//This player gets a bonus! How much?
	//Now we actually need to calculate
	
	//Get the total bonuses
	int32_t majBonus[NUM_CHAINS];
	int32_t minBonus[NUM_CHAINS];
	
	HALE_status_t status = HALE_OK;
	status = getChainBonuses(gs, NULL, majBonus, minBonus);
	if(status != HALE_OK)
	{
		PRINT_MSG("Error getting chain bonus values");
		return status;
	}
	
	//Majority holder
	if(playerHoldings == majHolding)
	{
		//Any other majority holders? Split evenly
		//Round DOWN to nearest 100
		if(majHolders > 1)
		{
			//Dividing by 100 and then multiplying is a convenient way of
			//rounding down... yay integer math!
			int32_t totalBonusDiv100 = (majBonus[chain] + minBonus[chain]) / 100;
			*bonus = (totalBonusDiv100/majHolders) * 100;
		}
		//Otherwise, this player is only majority holder- full bonus
		else
		{
			*bonus = majBonus[chain];
		}
	}
	//minority holder, exactly ONE majority holder
	//Just split the minority bonus
	else
	{
		//Again, use integer math to round down
		*bonus = ((minBonus[chain] / 100) / minHolders) * 100;
	}
	
	
	return HALE_OK;
}
