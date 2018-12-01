#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//getopt
#include <unistd.h>

#include "state.h"
#include "chain.h"
#include "player-types.h"
#include "util.h"

#include "config.h"
#ifdef HALE_ENABLE_PYTHON
#include <Python.h>
#endif //HALE_ENABLE_PYTHON

static void printHelp(char* name)
{
	printf("Usage: %s [OPTIONS]\r\n", name);
	printf("\r\n");
	printf("Option\t\tMeaning\r\n");
	printf(" -h\t\tShow this help and exit\r\n");
	printf(" -p <type>\tAdd a player of type <type>, from list below:\r\n");
	for(int i = 0; i < NUM_PLAYER_TYPES; i++)
	{
		printf("\t\t%s\r\n", PlayerTypes[i]->typeName);
	}
	printf(" -s <seed>\tForce random number generator seed\r\n");
	printf(" -v\t\tVerbose (IGNORED)\r\n");
}

int main(int argc, char* argv[])
{
	unsigned int randomSeed = time(NULL);
	uint8_t numPlayers = 0;
	uint8_t playerTypes[MAX_PLAYERS];
	memset(playerTypes, 0, MAX_PLAYERS*sizeof(playerTypes[0]));

	//Parse arguments and set up players
	{
		char c;
		while( (c = getopt(argc, argv, "p:vhs:")) != -1 )
		{
			switch(c)
			{
				//Add a player
				case 'p':
				{
					uint8_t playerType = 0xff;
					
					//Check validity
					for(int i = 0; i < NUM_PLAYER_TYPES; i++)
					{
						if(! strcmp(PlayerTypes[i]->typeName, optarg))
						{
							printf("Found %s in PlayerType list!\r\n", optarg);
							playerType = i;
							break;
						}
					}
					
					if(playerType < NUM_PLAYER_TYPES)
					{
						playerTypes[numPlayers] = playerType;
						numPlayers++;
					}
					else
					{
						PRINT_MSG_ARG("Invalid player type specified", optarg);
						return -1;
					}
					
					break;
				}
				
				//Verbose output
				case 'v':
				{
					printf("VERBOSE!\r\n");
					break;
				}
				
				//Set random seed (for testing and stuff)
				case 's':
				{
					if(! sscanf(optarg, "%d", &randomSeed))
					{
						printf("Couldn't parse provided random seed value: %s\r\n", optarg);
						return -1;
					}
					break;
				}
				
				//Print help
				case 'h':
				{
					printHelp(argv[0]);
					return 0;
					break;
				}
				
				//Handle missing arguments from flags requiring them
				case '?':
				{
					if(optopt == 'p')
					{
						printf("Flag 'p' requires an argument!\r\n");
						return -1;
					}
					else if(optopt == 's')
					{
						printf("Flag 's' requires an argument!\r\n");
						return -1;
					}
				}
				
				//You did a bad job
				default:
				{
					return -1;
					break;
				}
			}
		}
	}
	
	//Make sure we've got a valid number of players
	if(numPlayers < 2)
	{
		PRINT_MSG_INT("Not enough players specified- see -p option. Specified", numPlayers);
		return -1;
	}
	
	//FIXME: Initialize UI (someday...)
	
	//Initialize Python, if appropriate
#ifdef HALE_ENABLE_PYTHON
	Py_Initialize();
	//Hack to load modules from current directory
	PyRun_SimpleString(
	"import sys\n"
	"import os\n"
	"sys.path.append(os.getcwd())\n" );
#endif //HALE_ENABLE_PYTHON
	
	//Seed random number generator
	srand(randomSeed);
	
	//Run the actual game
	runGame(numPlayers, playerTypes);
	
	return 0;
}
