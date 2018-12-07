//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

#ifndef HALE_AI_PYTHON_CHARLIE_H
#define HALE_AI_PYTHON_CHARLIE_H

#include <stdint.h>

#include "state.h"

//Skeleton code to show how to implement a Python AI
//Note that Py_Initialize will have already been called, you do NOT need to call it here

//This is the name of the Python file you want to import
#define AI_PYTHON_CHARLIE_MOD_NAME "aipythoncharlie"

//Names of the functions for each action
#define AI_PYTHON_CHARLIE_FUNC_PLAY_TILE "pythonCharliePlayTile"
#define AI_PYTHON_CHARLIE_FUNC_FORM_CHAIN "pythonCharlieFormChain"
#define AI_PYTHON_CHARLIE_FUNC_MERGER_SURVIVOR "pythonCharlieMergerSurvivor"
#define AI_PYTHON_CHARLIE_FUNC_MERGER_ORDER "pythonCharlieMergerOrder"
#define AI_PYTHON_CHARLIE_FUNC_BUY_STOCK "pythonCharlieBuyStock"
#define AI_PYTHON_CHARLIE_FUNC_MERGER_TRADE "pythonCharlieMergerTrade"
#define AI_PYTHON_CHARLIE_FUNC_END_GAME "pythonCharlieEndGame"


//See player.h for an explanation of these functions

extern const PlayerActions_t pythonCharlieActions;



uint8_t pythonCharliePlayTile(GameState_t* gs, uint8_t playerNum);

chain_t pythonCharlieFormChain(GameState_t* gs, uint8_t playerNum);

chain_t pythonCharlieMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void pythonCharlieMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void pythonCharlieBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void pythonCharlieMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t pythonCharlieEndGame(GameState_t* gs, uint8_t playerNum);

#endif //HALE_AI_PYTHON_CHARLIE_H

#endif //HALE_ENABLE_PYTHON
