#include <string.h>

#include "board.h"
#include "chain.h"
#include "util.h"


const char* chainNames[NUM_CHAINS+1] = {"LUXOR", "TOWER", "WORLDWIDE", "AMERICAN", "FESTIVAL", "IMPERIAL", "CONTINENTAL", "NONE"};

const char* chainConsoleColors[NUM_CHAINS] = {ANSI_COLOR_RED, ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, ANSI_COLOR_GREEN, ANSI_COLOR_BLUE, ANSI_COLOR_MAGENTA, ANSI_COLOR_CYAN};


HALE_status_t getChainPricesPerShare(GameState_t* gs, int32_t* prices, uint8_t* sizesOut)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(prices, "prices");
	
	
	uint8_t sizes[NUM_CHAINS];
	getChainSizes(gs, sizes);
	
	//Cycle through the chains and calculate prices
	int i;
	for(i = 0; i < NUM_CHAINS; i++)
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
