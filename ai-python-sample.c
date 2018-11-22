//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

//Skeleton code to show how to implement a Python AI

#include <stdint.h>

#include <Python.h>

#include "ai-python-sample.h"
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
	pModName = PyUnicode_DecodeFSDefault(AI_PYTHON_SAMPLE_MOD_NAME);
	pModule = PyImport_Import(pModName);
	Py_DECREF(pModName);
	
	if(pModule == NULL)
	{
		PyErr_Print();
		PRINT_MSG("Failed to load module");
		return 1;
	}
	
	//Load all of the functions
	pyFuncPlayTile = PyObject_GetAttrString(pModule, AI_PYTHON_SAMPLE_FUNC_PLAY_TILE);
	pyFuncFormChain = PyObject_GetAttrString(pModule, AI_PYTHON_SAMPLE_FUNC_FORM_CHAIN);
	pyFuncMergerSurvivor = PyObject_GetAttrString(pModule, AI_PYTHON_SAMPLE_FUNC_MERGER_SURVIVOR);
	pyFuncMergerOrder = PyObject_GetAttrString(pModule, AI_PYTHON_SAMPLE_FUNC_MERGER_ORDER);
	pyFuncBuyStock = PyObject_GetAttrString(pModule, AI_PYTHON_SAMPLE_FUNC_BUY_STOCK);
	pyFuncMergerTrade = PyObject_GetAttrString(pModule, AI_PYTHON_SAMPLE_FUNC_MERGER_TRADE);
	pyFuncEndGame = PyObject_GetAttrString(pModule, AI_PYTHON_SAMPLE_FUNC_END_GAME);
	
	//FIXME: Check if any are NULL or uncallable
	
	return 0;
}

PlayerActions_t pythonSampleActions = {	.playTile = pythonSamplePlayTile,
					.formChain = pythonSampleFormChain,
					.mergerSurvivor = pythonSampleMergerSurvivor,
					.mergerOrder = pythonSampleMergerOrder,
					.buyStock = pythonSampleBuyStock,
					.mergerTrade = pythonSampleMergerTrade,
					.endGame = pythonSampleEndGame
				};


uint8_t pythonSamplePlayTile(GameState_t* gs, uint8_t playerNum)
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

chain_t pythonSampleFormChain(GameState_t* gs, uint8_t playerNum)
{
	//FIXME: Implement
	PyObject* pyGs = gsToPy(gs);
	
	//FIXME: See boilerplate in pythonSamplePlayTile until I write this
	
	//Clean up references
	//Note that this likely does NOT include pyGs, since PyTuple_SetItem is a special case that steals a reference, and pyGs will thus be implicitly cleaned up (?)
	//Py_XDECREF(pyGs);
	return CHAIN_NONE;
}

chain_t pythonSampleMergerSurvivor(GameState_t* gs, uint8_t playerNum, uint8_t* options)
{
	//FIXME: Implement
	PyObject* pyGs = gsToPy(gs);
	
	//FIXME: See boilerplate in pythonSamplePlayTile until I write this
	
	//Clean up references
	//Note that this likely does NOT include pyGs, since PyTuple_SetItem is a special case that steals a reference, and pyGs will thus be implicitly cleaned up (?)
	//Py_XDECREF(pyGs);
	return CHAIN_NONE;
}

void pythonSampleMergerOrder(GameState_t* gs, uint8_t playerNum, chain_t survivor, uint8_t* options)
{
	//FIXME: Implement
	PyObject* pyGs = gsToPy(gs);
	
	//FIXME: See boilerplate in pythonSamplePlayTile until I write this
	
	//Clean up references
	//Note that this likely does NOT include pyGs, since PyTuple_SetItem is a special case that steals a reference, and pyGs will thus be implicitly cleaned up (?)
	//Py_XDECREF(pyGs);
	return;
}

void pythonSampleBuyStock(GameState_t* gs, uint8_t playerNum, uint8_t* toBuy)
{
	//FIXME: Implement
	PyObject* pyGs = gsToPy(gs);
	
	//FIXME: See boilerplate in pythonSamplePlayTile until I write this
	
	//Clean up references
	//Note that this likely does NOT include pyGs, since PyTuple_SetItem is a special case that steals a reference, and pyGs will thus be implicitly cleaned up (?)
	//Py_XDECREF(pyGs);
	return;
}

void pythonSampleMergerTrade(GameState_t* gs, uint8_t playerNum, chain_t survivor, chain_t defunct, uint8_t* tradeFor, uint8_t* sell)
{
	//FIXME: Implement
	PyObject* pyGs = gsToPy(gs);
	
	//FIXME: See boilerplate in pythonSamplePlayTile until I write this
	
	//Clean up references
	//Note that this likely does NOT include pyGs, since PyTuple_SetItem is a special case that steals a reference, and pyGs will thus be implicitly cleaned up (?)
	//Py_XDECREF(pyGs);
	return;
}

uint8_t pythonSampleEndGame(GameState_t* gs, uint8_t playerNum)
{
	//FIXME: Implement
	PyObject* pyGs = gsToPy(gs);
	
	//FIXME: See boilerplate in pythonSamplePlayTile until I write this
	
	//Clean up references
	//Note that this likely does NOT include pyGs, since PyTuple_SetItem is a special case that steals a reference, and pyGs will thus be implicitly cleaned up (?)
	//Py_XDECREF(pyGs);
	return 0;
}

#endif //HALE_ENABLE_PYTHON
