//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

#ifndef HALE_AI_PYTHON_JAMES_H
#define HALE_AI_PYTHON_JAMES_H

#include <stdint.h>

#include "state.h"

//Skeleton code to show how to implement a Python AI
//Note that Py_Initialize will have already been called, you do NOT need to call it here

//This is the name of the Python file you want to import
#define AI_PYTHON_JAMES_MOD_NAME "aipythonjames"

//Names of the functions for each action
#define AI_PYTHON_JAMES_FUNC_PLAY_TILE "pythonJamesPlayTile"
#define AI_PYTHON_JAMES_FUNC_FORM_CHAIN "pythonJamesFormChain"
#define AI_PYTHON_JAMES_FUNC_MERGER_SURVIVOR "pythonJamesMergerSurvivor"
#define AI_PYTHON_JAMES_FUNC_MERGER_ORDER "pythonJamesMergerOrder"
#define AI_PYTHON_JAMES_FUNC_BUY_STOCK "pythonJamesBuyStock"
#define AI_PYTHON_JAMES_FUNC_MERGER_TRADE "pythonJamesMergerTrade"
#define AI_PYTHON_JAMES_FUNC_END_GAME "pythonJamesEndGame"


//See player.h for an explanation of these functions

extern const PlayerActions_t pythonJamesActions;



uint8_t pythonJamesPlayTile(GameState_t* gs, uint8_t playerNum);

chain_t pythonJamesFormChain(GameState_t* gs, uint8_t playerNum);

chain_t pythonJamesMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void pythonJamesMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void pythonJamesBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void pythonJamesMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t pythonJamesEndGame(GameState_t* gs, uint8_t playerNum);

#endif //HALE_AI_PYTHON_JAMES_H

#endif //HALE_ENABLE_PYTHON
