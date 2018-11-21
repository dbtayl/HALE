//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

#ifndef HALE_AI_PYTHON_SAMPLE_H
#define HALE_AI_PYTHON_SAMPLE_H

#include <stdint.h>

#include "state.h"

//Skeleton code to show how to implement a Python AI
//Note that Py_Initialize will have already been called, you do NOT need to call it here

//This is the name of the Python file you want to import
#define AI_PYTHON_SAMPLE_MOD_NAME "ai-python-sample"

//Names of the functions for each action
#define AI_PYTHON_SAMPLE_FUNC_PLAY_TILE "pythonSamplePlayTile"
#define AI_PYTHON_SAMPLE_FUNC_FORM_CHAIN "pythonSampleFormChain"
#define AI_PYTHON_SAMPLE_FUNC_MERGER_SURVIVOR "pythonSampleMergerSurvivor"
#define AI_PYTHON_SAMPLE_FUNC_MERGER_ORDER "pythonSampleMergerOrder"
#define AI_PYTHON_SAMPLE_FUNC_BUY_STOCK "pythonSampleBuyStock"
#define AI_PYTHON_SAMPLE_FUNC_MERGER_TRADE "pythonSampleMergerTrade"
#define AI_PYTHON_SAMPLE_FUNC_END_GAME "pythonSampleEndGame"


//See player.h for an explanation of these functions

extern PlayerActions_t pythonSampleActions;



uint8_t pythonSamplePlayTile(GameState_t* gs, uint8_t playerNum);

chain_t pythonSampleFormChain(GameState_t* gs, uint8_t playerNum);

chain_t pythonSampleMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options);

void pythonSampleMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options);

void pythonSampleBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy);

void pythonSampleMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell);

uint8_t pythonSampleEndGame(GameState_t* gs, uint8_t playerNum);

#endif //HALE_AI_PYTHON_SAMPLE_H

#endif //HALE_ENABLE_PYTHON
