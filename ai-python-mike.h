//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

#ifndef HALE_AI_PYTHON_MIKE_H
#define HALE_AI_PYTHON_MIKE_H

#include <stdint.h>

#include "state.h"

//Skeleton code to show how to implement a Python AI
//Note that Py_Initialize will have already been called, you do NOT need to call it here

//This is the name of the Python file you want to import
#define AI_PYTHON_MIKE_MOD_NAME "aipythonmike"

//Names of the functions for each action
#define AI_PYTHON_MIKE_FUNC_PLAY_TILE "pythonMikePlayTile"
#define AI_PYTHON_MIKE_FUNC_FORM_CHAIN "pythonMikeFormChain"
#define AI_PYTHON_MIKE_FUNC_MERGER_SURVIVOR "pythonMikeMergerSurvivor"
#define AI_PYTHON_MIKE_FUNC_MERGER_ORDER "pythonMikeMergerOrder"
#define AI_PYTHON_MIKE_FUNC_BUY_STOCK "pythonMikeBuyStock"
#define AI_PYTHON_MIKE_FUNC_MERGER_TRADE "pythonMikeMergerTrade"
#define AI_PYTHON_MIKE_FUNC_END_GAME "pythonMikeEndGame"


//See player.h for an explanation of these functions

extern const PlayerActions_t pythonMikeActions;



uint8_t pythonMikePlayTile(GameState_t* gs, uint8_t playerNum);

chain_t pythonMikeFormChain(GameState_t* gs, uint8_t playerNum);

chain_t pythonMikeMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void pythonMikeMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void pythonMikeBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void pythonMikeMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t pythonMikeEndGame(GameState_t* gs, uint8_t playerNum);

#endif //HALE_AI_PYTHON_MIKE_H

#endif //HALE_ENABLE_PYTHON
