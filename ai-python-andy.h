//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

#ifndef HALE_AI_PYTHON_ANDY_H
#define HALE_AI_PYTHON_ANDY_H

#include <stdint.h>

#include "state.h"

//Skeleton code to show how to implement a Python AI
//Note that Py_Initialize will have already been called, you do NOT need to call it here

//This is the name of the Python file you want to import
#define AI_PYTHON_ANDY_MOD_NAME "aipythonandy"

//Names of the functions for each action
#define AI_PYTHON_ANDY_FUNC_PLAY_TILE "pythonAndyPlayTile"
#define AI_PYTHON_ANDY_FUNC_FORM_CHAIN "pythonAndyFormChain"
#define AI_PYTHON_ANDY_FUNC_MERGER_SURVIVOR "pythonAndyMergerSurvivor"
#define AI_PYTHON_ANDY_FUNC_MERGER_ORDER "pythonAndyMergerOrder"
#define AI_PYTHON_ANDY_FUNC_BUY_STOCK "pythonAndyBuyStock"
#define AI_PYTHON_ANDY_FUNC_MERGER_TRADE "pythonAndyMergerTrade"
#define AI_PYTHON_ANDY_FUNC_END_GAME "pythonAndyEndGame"


//See player.h for an explanation of these functions

extern const PlayerActions_t pythonAndyActions;



uint8_t pythonAndyPlayTile(GameState_t* gs, uint8_t playerNum);

chain_t pythonAndyFormChain(GameState_t* gs, uint8_t playerNum);

chain_t pythonAndyMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void pythonAndyMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void pythonAndyBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void pythonAndyMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t pythonAndyEndGame(GameState_t* gs, uint8_t playerNum);

#endif //HALE_AI_PYTHON_ANDY_H

#endif //HALE_ENABLE_PYTHON
