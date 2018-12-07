//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

//Skeleton code to show how to implement a Python AI

#include <stdint.h>

#include <Python.h>

#include "ai-python-mike.h"
#include "python-wrapper.h"
#include "state.h"
#include "util.h"

//Local state variables- only need to be initialized once
static PyObject* pModName = NULL;
static PyObject* pModule = NULL;
static PyObject* pyFuncPlayTile = NULL;
static PyObject* pyFuncFormChain = NULL;
static PyObject* pyFuncMergerSurvivor = NULL;
static PyObject* pyFuncMergerOrder = NULL;
static PyObject* pyFuncBuyStock = NULL;
static PyObject* pyFuncMergerTrade = NULL;
static PyObject* pyFuncEndGame = NULL;

//Local function to initialize the Python state

//Note that this is a quick and dirty implementation- does NOT check if things
//are NULL before trying to load them!

//NOTE: There will theoretically be an "init" function added to the player
//function sets, where this can be taken care of more tidily
static uint8_t initPython()
{
	//Try to load the module
	pModName = PyUnicode_DecodeFSDefault(AI_PYTHON_MIKE_MOD_NAME);
	pModule = PyImport_Import(pModName);
	Py_DECREF(pModName);
	
	if(pModule == NULL)
	{
		PyErr_Print();
		PRINT_MSG("Failed to load module");
		return 1;
	}
	
	//Load all of the functions
	pyFuncPlayTile = PyObject_GetAttrString(pModule, AI_PYTHON_MIKE_FUNC_PLAY_TILE);
	pyFuncFormChain = PyObject_GetAttrString(pModule, AI_PYTHON_MIKE_FUNC_FORM_CHAIN);
	pyFuncMergerSurvivor = PyObject_GetAttrString(pModule, AI_PYTHON_MIKE_FUNC_MERGER_SURVIVOR);
	pyFuncMergerOrder = PyObject_GetAttrString(pModule, AI_PYTHON_MIKE_FUNC_MERGER_ORDER);
	pyFuncBuyStock = PyObject_GetAttrString(pModule, AI_PYTHON_MIKE_FUNC_BUY_STOCK);
	pyFuncMergerTrade = PyObject_GetAttrString(pModule, AI_PYTHON_MIKE_FUNC_MERGER_TRADE);
	pyFuncEndGame = PyObject_GetAttrString(pModule, AI_PYTHON_MIKE_FUNC_END_GAME);
	
	//FIXME: Check if any are NULL or uncallable
	
	return 0;
}

const PlayerActions_t pythonMikeActions = {	.playTile = pythonMikePlayTile,
					.formChain = pythonMikeFormChain,
					.mergerSurvivor = pythonMikeMergerSurvivor,
					.mergerOrder = pythonMikeMergerOrder,
					.buyStock = pythonMikeBuyStock,
					.mergerTrade = pythonMikeMergerTrade,
					.endGame = pythonMikeEndGame,
					.typeName = "aipythonmike"
				};


uint8_t pythonMikePlayTile(GameState_t* gs, uint8_t playerNum)
{
	//The tile we want to return. Theoretically this will be properly
	//populated by the Python function we call
	uint8_t tile = 0;
	
	//If we haven't initialized our local state... do so
	//NOTE: This will hopefully be unnecessary in the future, when an
	//init function is included in the list of player functions
	if(!pModule || !pyFuncPlayTile)
	{
		//FIXME: Error checking
		initPython();
	}
	
	//Make a copy of game state
	PyObject* pyGs = gsToPy(gs);
	
	//List of arguments
	PyObject* pArgs = PyTuple_New(2);
	//pyGs is the first argument- no need to build that
	//playerNum is the second argument- need to build that
	PyObject* pPlayerNum = PyLong_FromLong(playerNum);
	
	//Assign arguments
	PyTuple_SetItem(pArgs, 0, pyGs);
	PyTuple_SetItem(pArgs, 1, pPlayerNum);
	
	//Do NOT decrement references- PyTuple_SetItem is a special case that STEALS a reference instead of incrementing
	//Decrement references
	//Py_XDECREF(pPlayerNum);
	//Py_XDECREF(pyGs);
	
	//Call python code
	PyObject* pRetVal = PyObject_CallObject(pyFuncPlayTile, pArgs);
	
	//Parse out returned value
	if (pRetVal != NULL)
	{
		//If the returned value is, in fact, an integer
		if(PyLong_Check(pRetVal))
		{
			tile = PyLong_AsLong(pRetVal);
		}
		
		//Do NOT decref here- done later
	}
	
	//Clean up references
	//pPlayerNum and pyGs are cleaned up when pArgs is cleaned up (?)
	Py_XDECREF(pArgs);
	
	//XDECREF handles NULL pointers for us- no need to check for NULL
	Py_XDECREF(pRetVal);
	
	//May or may not be valid... too bad if it's not!
	return tile;
}

chain_t pythonMikeFormChain(GameState_t* gs, uint8_t playerNum)
{
	//The chain we want to return. Theoretically this will be properly
	//populated by the Python function we call
	chain_t chain = 0;
	
	//If we haven't initialized our local state... do so
	//NOTE: This will hopefully be unnecessary in the future, when an
	//init function is included in the list of player functions
	if(!pModule || !pyFuncFormChain)
	{
		//FIXME: Error checking
		initPython();
	}
	
	//Make a copy of game state
	PyObject* pyGs = gsToPy(gs);
	
	//List of arguments
	PyObject* pArgs = PyTuple_New(2);
	//pyGs is the first argument- no need to build that
	//playerNum is the second argument- need to build that
	PyObject* pPlayerNum = PyLong_FromLong(playerNum);
	
	//Assign arguments
	PyTuple_SetItem(pArgs, 0, pyGs);
	PyTuple_SetItem(pArgs, 1, pPlayerNum);
	
	//Do NOT decrement references- PyTuple_SetItem is a special case that STEALS a reference instead of incrementing
	//Decrement references
	//Py_XDECREF(pPlayerNum);
	//Py_XDECREF(pyGs);
	
	//Call python code
	PyObject* pRetVal = PyObject_CallObject(pyFuncFormChain, pArgs);
	
	//Parse out returned value
	if (pRetVal != NULL)
	{
		//If the returned value is, in fact, an integer
		if(PyLong_Check(pRetVal))
		{
			chain = PyLong_AsLong(pRetVal);
		}
		
		//Do NOT decref here- done later
	}
	
	//Clean up references
	//pPlayerNum and pyGs are cleaned up when pArgs is cleaned up (?)
	Py_XDECREF(pArgs);
	
	//XDECREF handles NULL pointers for us- no need to check for NULL
	Py_XDECREF(pRetVal);
	
	//May or may not be valid... too bad if it's not!
	return chain;
}

chain_t pythonMikeMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options)
{
	//The chain we want to return. Theoretically this will be properly
	//populated by the Python function we call
	chain_t chain = 0;
	
	//If we haven't initialized our local state... do so
	//NOTE: This will hopefully be unnecessary in the future, when an
	//init function is included in the list of player functions
	if(!pModule || !pyFuncFormChain)
	{
		//FIXME: Error checking
		initPython();
	}
	
	//Make a copy of game state
	PyObject* pyGs = gsToPy(gs);
	
	//List of arguments
	PyObject* pArgs = PyTuple_New(3);
	//pyGs is the first argument- no need to build that
	//playerNum is the second argument- need to build that
	PyObject* pPlayerNum = PyLong_FromLong(playerNum);
	PyObject* pOptions = PyList_New(NUM_CHAINS);
	
	for(int i = 0; i < NUM_CHAINS; i++)
	{
		PyObject* pInt = PyLong_FromLong(options[i]);
		PyList_SET_ITEM(pOptions, i, pInt);
		//NOTE: PyList_SetItem steals a reference- no need to decrement!
	}
	
	//Assign arguments
	PyTuple_SetItem(pArgs, 0, pyGs);
	PyTuple_SetItem(pArgs, 1, pPlayerNum);
	PyTuple_SetItem(pArgs, 2, pOptions);
	
	//Do NOT decrement references- PyTuple_SetItem is a special case that STEALS a reference instead of incrementing
	//Decrement references
	//Py_XDECREF(pPlayerNum);
	//Py_XDECREF(pyGs);
	
	//Call python code
	PyObject* pRetVal = PyObject_CallObject(pyFuncFormChain, pArgs);
	
	//Parse out returned value
	if (pRetVal != NULL)
	{
		//If the returned value is, in fact, an integer
		if(PyLong_Check(pRetVal))
		{
			chain = PyLong_AsLong(pRetVal);
		}
		
		//Do NOT decref here- done later
	}
	
	//Clean up references
	//pPlayerNum and pyGs are cleaned up when pArgs is cleaned up (?)
	Py_XDECREF(pArgs);
	
	//XDECREF handles NULL pointers for us- no need to check for NULL
	Py_XDECREF(pRetVal);
	
	//May or may not be valid... too bad if it's not!
	return chain;
}

void pythonMikeMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options)
{
	//If we haven't initialized our local state... do so
	//NOTE: This will hopefully be unnecessary in the future, when an
	//init function is included in the list of player functions
	if(!pModule || !pyFuncFormChain)
	{
		//FIXME: Error checking
		initPython();
	}
	
	//Make a copy of game state
	PyObject* pyGs = gsToPy(gs);
	
	//List of arguments
	PyObject* pArgs = PyTuple_New(3);
	//pyGs is the first argument- no need to build that
	//playerNum is the second argument- need to build that
	PyObject* pPlayerNum = PyLong_FromLong(playerNum);
	PyObject* pSurvivor = PyLong_FromLong(survivor);
	
	//Assign arguments
	PyTuple_SetItem(pArgs, 0, pyGs);
	PyTuple_SetItem(pArgs, 1, pPlayerNum);
	PyTuple_SetItem(pArgs, 2, pSurvivor);
	
	//Do NOT decrement references- PyTuple_SetItem is a special case that STEALS a reference instead of incrementing
	//Decrement references
	//Py_XDECREF(pPlayerNum);
	//Py_XDECREF(pyGs);
	
	//Call python code
	PyObject* pRetVal = PyObject_CallObject(pyFuncFormChain, pArgs);
	
	//Parse out returned value
	//If the returned value is, in fact, a list
	if (pRetVal != NULL && PyList_Check(pRetVal))
	{
		//...and it's the right length
		if(PyList_Size(pRetVal) == NUM_CHAINS)
		{
			//...and it's a list of integers
			if(PyLong_Check(PyList_GetItem(pRetVal, 0)))
			{
				//Extract all of the values
				for(int i = 0; i < NUM_CHAINS; i++)
				{
					options[i] = (uint8_t)PyLong_AsLong(PyList_GetItem(pRetVal, i));
				}
			}
		}
		
		//Do NOT decref here- done later
	}
	
	//Clean up references
	//pPlayerNum and pyGs are cleaned up when pArgs is cleaned up (?)
	Py_XDECREF(pArgs);
	
	//XDECREF handles NULL pointers for us- no need to check for NULL
	Py_XDECREF(pRetVal);
	
	//Return nothing- options should have been properly populated
}

void pythonMikeBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy)
{
	//If we haven't initialized our local state... do so
	//NOTE: This will hopefully be unnecessary in the future, when an
	//init function is included in the list of player functions
	if(!pModule || !pyFuncFormChain)
	{
		//FIXME: Error checking
		initPython();
	}
	
	//Make a copy of game state
	PyObject* pyGs = gsToPy(gs);
	
	//List of arguments
	PyObject* pArgs = PyTuple_New(2);
	//pyGs is the first argument- no need to build that
	//playerNum is the second argument- need to build that
	PyObject* pPlayerNum = PyLong_FromLong(playerNum);
	
	//Assign arguments
	PyTuple_SetItem(pArgs, 0, pyGs);
	PyTuple_SetItem(pArgs, 1, pPlayerNum);
	
	//Do NOT decrement references- PyTuple_SetItem is a special case that STEALS a reference instead of incrementing
	//Decrement references
	//Py_XDECREF(pPlayerNum);
	//Py_XDECREF(pyGs);
	
	//Call python code
	PyObject* pRetVal = PyObject_CallObject(pyFuncFormChain, pArgs);
	
	//Parse out returned value
	//If the returned value is, in fact, a list
	if (pRetVal != NULL && PyList_Check(pRetVal))
	{
		//...and it's the right length
		if(PyList_Size(pRetVal) == NUM_CHAINS)
		{
			//...and it's a list of integers
			if(PyLong_Check(PyList_GetItem(pRetVal, 0)))
			{
				//Extract all of the values
				for(int i = 0; i < NUM_CHAINS; i++)
				{
					toBuy[i] = (uint8_t)PyLong_AsLong(PyList_GetItem(pRetVal, i));
				}
			}
		}
		
		//Do NOT decref here- done later
	}
	
	//Clean up references
	//pPlayerNum and pyGs are cleaned up when pArgs is cleaned up (?)
	Py_XDECREF(pArgs);
	
	//XDECREF handles NULL pointers for us- no need to check for NULL
	Py_XDECREF(pRetVal);
	
	//Return nothing- toBuy should have been properly populated
}

void pythonMikeMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell)
{
	//If we haven't initialized our local state... do so
	//NOTE: This will hopefully be unnecessary in the future, when an
	//init function is included in the list of player functions
	if(!pModule || !pyFuncFormChain)
	{
		//FIXME: Error checking
		initPython();
	}
	
	//Make a copy of game state
	PyObject* pyGs = gsToPy(gs);
	
	//List of arguments
	PyObject* pArgs = PyTuple_New(4);
	//pyGs is the first argument- no need to build that
	//playerNum is the second argument- need to build that
	PyObject* pPlayerNum = PyLong_FromLong(playerNum);
	PyObject* pSurvivor = PyLong_FromLong(survivor);
	PyObject* pDefunct = PyLong_FromLong(defunct);
	
	//Assign arguments
	PyTuple_SetItem(pArgs, 0, pyGs);
	PyTuple_SetItem(pArgs, 1, pPlayerNum);
	PyTuple_SetItem(pArgs, 2, pSurvivor);
	PyTuple_SetItem(pArgs, 3, pDefunct);
	
	//Do NOT decrement references- PyTuple_SetItem is a special case that STEALS a reference instead of incrementing
	//Decrement references
	//Py_XDECREF(pPlayerNum);
	//Py_XDECREF(pyGs);
	
	//Call python code
	PyObject* pRetVal = PyObject_CallObject(pyFuncFormChain, pArgs);
	
	//Parse out returned value
	//If the returned value is, in fact, a tuple
	if (pRetVal != NULL && PyTuple_Check(pRetVal))
	{
		//...and it's the right length
		if(PyTuple_Size(pRetVal) == 2)
		{
			//...and it's a tuple of integers
			//BORROWED references!
			PyObject* pTradeFor = PyTuple_GetItem(pRetVal, 0);
			PyObject* pSell = PyTuple_GetItem(pRetVal, 1);
			if(PyLong_Check(pTradeFor) && PyLong_Check(pSell))
			{
				*tradeFor = (uint8_t)PyLong_AsLong(pTradeFor);
				*sell = (uint8_t)PyLong_AsLong(pSell);
			}
		}
		
		//Do NOT decref here- done later
	}
	
	//Clean up references
	//pPlayerNum and pyGs are cleaned up when pArgs is cleaned up (?)
	Py_XDECREF(pArgs);
	
	//XDECREF handles NULL pointers for us- no need to check for NULL
	Py_XDECREF(pRetVal);
	
	//Return nothing- tradeFor and sell should have been properly populated
}

uint8_t pythonMikeEndGame(GameState_t* gs, uint8_t playerNum)
{
	//Whether we wan ot end the game. Theoretically this will be properly
	//populated by the Python function we call
	uint8_t endGame = 0;
	
	//If we haven't initialized our local state... do so
	//NOTE: This will hopefully be unnecessary in the future, when an
	//init function is included in the list of player functions
	if(!pModule || !pyFuncEndGame)
	{
		//FIXME: Error checking
		initPython();
	}
	
	//Make a copy of game state
	PyObject* pyGs = gsToPy(gs);
	
	//List of arguments
	PyObject* pArgs = PyTuple_New(2);
	//pyGs is the first argument- no need to build that
	//playerNum is the second argument- need to build that
	PyObject* pPlayerNum = PyLong_FromLong(playerNum);
	
	//Assign arguments
	PyTuple_SetItem(pArgs, 0, pyGs);
	PyTuple_SetItem(pArgs, 1, pPlayerNum);
	
	//Do NOT decrement references- PyTuple_SetItem is a special case that STEALS a reference instead of incrementing
	//Decrement references
	//Py_XDECREF(pPlayerNum);
	//Py_XDECREF(pyGs);
	
	//Call python code
	PyObject* pRetVal = PyObject_CallObject(pyFuncEndGame, pArgs);
	
	//Parse out returned value
	if (pRetVal != NULL)
	{
		//If the returned value is, in fact, an integer
		if(PyLong_Check(pRetVal))
		{
			endGame = PyLong_AsLong(pRetVal);
		}
		
		//Do NOT decref here- done later
	}
	
	//Clean up references
	//pPlayerNum and pyGs are cleaned up when pArgs is cleaned up (?)
	Py_XDECREF(pArgs);
	
	//XDECREF handles NULL pointers for us- no need to check for NULL
	Py_XDECREF(pRetVal);
	
	//May or may not be valid... too bad if it's not!
	return endGame;
}

#endif //HALE_ENABLE_PYTHON
