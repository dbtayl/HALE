#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "chain.h"
#include "util.h"

int main()
{
	//FIXME: Figure out player configuration (number of players, human/AI)
	PRINT_MSG("FIXME: Need to identify player configuration: number of players, human/AI, ...");
	
	//FIXME: Initialize UI
	
	//Seed random number generator
	srand(time(NULL));
	
	//Run the actual game
	//FIXME: Need to actually use the number of players...
	PRINT_MSG("FIXME: Need to pass real number of players to runGame(), not a constant 4");
	runGame(4);
	return 0;
}
