//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

#ifndef HALE_AI_PYTHON_JIM_H
#define HALE_AI_PYTHON_JIM_H

#include <stdint.h>

#include "state.h"

//Skeleton code to show how to implement a Python AI
//Note that Py_Initialize will have already been called, you do NOT need to call it here

//This is the name of the Python file you want to import
#define AI_PYTHON_JIM_MOD_NAME "aipythonjim"

//Names of the functions for each action
#define AI_PYTHON_JIM_FUNC_PLAY_TILE "pythonJimPlayTile"
#define AI_PYTHON_JIM_FUNC_FORM_CHAIN "pythonJimFormChain"
#define AI_PYTHON_JIM_FUNC_MERGER_SURVIVOR "pythonJimMergerSurvivor"
#define AI_PYTHON_JIM_FUNC_MERGER_ORDER "pythonJimMergerOrder"
#define AI_PYTHON_JIM_FUNC_BUY_STOCK "pythonJimBuyStock"
#define AI_PYTHON_JIM_FUNC_MERGER_TRADE "pythonJimMergerTrade"
#define AI_PYTHON_JIM_FUNC_END_GAME "pythonJimEndGame"


//See player.h for an explanation of these functions

extern const PlayerActions_t pythonJimActions;



uint8_t pythonJimPlayTile(GameState_t* gs, uint8_t playerNum);

chain_t pythonJimFormChain(GameState_t* gs, uint8_t playerNum);

chain_t pythonJimMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void pythonJimMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void pythonJimBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void pythonJimMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t pythonJimEndGame(GameState_t* gs, uint8_t playerNum);

#endif //HALE_AI_PYTHON_JIM_H

#endif //HALE_ENABLE_PYTHON
