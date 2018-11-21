//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

#include <Python.h>

#include "python-wrapper.h"
#include "state.h"
#include "util.h"

//Copies a GameSate_t* object into a Python PyObject... aka a big ol' dictionary
//This will create a NEW reference to a dictionary!
//This does NOT do any sanitizing!
PyObject* gsToPy(GameState_t* gs)
{
	PyObject* pGsDict = PyDict_New();
	if(!pGsDict)
	{
		PRINT_MSG("Failed to make dict");
		return NULL;
	}
	
	
	//Add numPlayers
	{
		PyObject* pNumPlayers = PyLong_FromLong(gs->numPlayers);
		PyObject* pKey = PyBytes_FromString("numPlayers");
		
		if( !pNumPlayers || !pKey || !PyDict_SetItem(pGsDict, pKey, pNumPlayers))
		{
			PRINT_MSG("Failed to add numPlayers");
			Py_XDECREF(pNumPlayers);
			Py_XDECREF(pKey);
			Py_DECREF(pGsDict);
			return NULL;
		}
		
		Py_DECREF(pKey);
		Py_DECREF(pNumPlayers);
	}
	
	//Add board
	{
		PyObject* pBoard = PyList_New(BOARD_TILES);
		PyObject* pKey = PyBytes_FromString("board");
		
		//Actually assign each tile in the list
		//FIXME: Error checking
		for(int i = 0; i < BOARD_TILES; i++)
		{
			PyObject* pInt = PyLong_FromLong(gs->board[i]);
			PyList_SET_ITEM(pBoard, i, pInt);
			Py_DECREF(pInt);
		}
		//Add board to the dict
		if( !pBoard || !pKey || !PyDict_SetItem(pGsDict, pKey, pBoard))
		{
			PRINT_MSG("Failed to add board");
			Py_XDECREF(pBoard);
			Py_XDECREF(pKey);
			Py_DECREF(pGsDict);
			return NULL;
		}
		
		Py_DECREF(pKey);
		Py_DECREF(pBoard);
	}
	
	//Copy of board code for remainingStocks
	{
		PyObject* pRemainingStocks = PyList_New(NUM_CHAINS);
		PyObject* pKey = PyBytes_FromString("remainingStocks");
		
		//Actually assign each tile in the list
		//FIXME: Error checking
		for(int i = 0; i < NUM_CHAINS; i++)
		{
			PyObject* pInt = PyLong_FromLong(gs->remainingStocks[i]);
			PyList_SET_ITEM(pRemainingStocks, i, pInt);
			Py_DECREF(pInt);
		}
		//Add to the dict
		if( !pRemainingStocks || !pKey || !PyDict_SetItem(pGsDict, pKey, pRemainingStocks))
		{
			PRINT_MSG("Failed to add remainingStocks");
			Py_XDECREF(pRemainingStocks);
			Py_XDECREF(pKey);
			Py_DECREF(pGsDict);
			return NULL;
		}
		
		Py_DECREF(pKey);
		Py_DECREF(pRemainingStocks);
	}
	
	//Copy of board code for tilePool
	{
		PyObject* pTilePool = PyList_New(BOARD_TILES);
		PyObject* pKey = PyBytes_FromString("tilePool");
		
		//Actually assign each tile in the list
		//FIXME: Error checking
		for(int i = 0; i < BOARD_TILES; i++)
		{
			PyObject* pInt = PyLong_FromLong(gs->tilePool[i]);
			PyList_SET_ITEM(pTilePool, i, pInt);
			Py_DECREF(pInt);
		}
		//Add board to the dict
		if( !pTilePool || !pKey || !PyDict_SetItem(pGsDict, pKey, pTilePool))
		{
			PRINT_MSG("Failed to add tilePool");
			Py_XDECREF(pTilePool);
			Py_XDECREF(pKey);
			Py_DECREF(pGsDict);
			return NULL;
		}
		
		Py_DECREF(pKey);
		Py_DECREF(pTilePool);
	}
	
	//Add remainingTiles
	{
		PyObject* pRemainingTiles = PyLong_FromLong(gs->remainingTiles);
		PyObject* pKey = PyBytes_FromString("remainingTiles");
		
		if( !pRemainingTiles || !pKey || !PyDict_SetItem(pGsDict, pKey, pRemainingTiles))
		{
			PRINT_MSG("Failed to add remainingTiles");
			Py_XDECREF(pRemainingTiles);
			Py_XDECREF(pKey);
			Py_DECREF(pGsDict);
			return NULL;
		}
		
		Py_DECREF(pKey);
		Py_DECREF(pRemainingTiles);
	}
	
	//Add currentPlayer
	{
		PyObject* pCurrentPlayer = PyLong_FromLong(gs->currentPlayer);
		PyObject* pKey = PyBytes_FromString("currentPlayer");
		
		if( !pCurrentPlayer || !pKey || !PyDict_SetItem(pGsDict, pKey, pCurrentPlayer))
		{
			PRINT_MSG("Failed to add currentPlayer");
			Py_XDECREF(pCurrentPlayer);
			Py_XDECREF(pKey);
			Py_DECREF(pGsDict);
			return NULL;
		}
		
		Py_DECREF(pKey);
		Py_DECREF(pCurrentPlayer);
	}
	
	//Copy array of player states (sans function calls)
	{
		PyObject* pPlayers = PyList_New(gs->numPlayers);
		PyObject* pKey = PyBytes_FromString("players");
		
		//Actually assign all of the player values...
		//FIXME: Error checking
		for(int i = 0; i < gs->numPlayers; i++)
		{
			//each player is ANOTHER dictionary:
			//cash
			//stocks (array)
			//tiles (array)
			//actions (???)
			//name (string)
			
			PyObject* pPDict = PyDict_New();
			if(!pPDict)
			{
				PRINT_MSG("Failed to create player dict");
				Py_XDECREF(pPlayers);
				Py_XDECREF(pKey);
				return NULL;
			}
			
			//Player cash
			//FIXME: Error checking
			PyObject* pCash = PyLong_FromLong(gs->players[i].cash);
			PyObject* pCashKey = PyBytes_FromString("cash");
			PyDict_SetItem(pPDict, pCashKey, pCash);
			Py_DECREF(pCash);
			Py_DECREF(pCashKey);
			
			
			//Stocks- another loop!
			//FIXME: Error checking
			PyObject* pStocks = PyList_New(NUM_CHAINS);
			PyObject* pStocksKey = PyBytes_FromString("stocks");
			for(int j = 0; j < NUM_CHAINS; j++)
			{
				PyObject* pStock = PyLong_FromLong(gs->players[i].stocks[j]);
				PyList_SET_ITEM(pStocks, j, pStock);
				Py_XDECREF(pStock);
			}
			//Clean up references
			if( !pStocks || !pStocksKey || !PyDict_SetItem(pGsDict, pStocksKey, pStocks))
			{
				PRINT_MSG("Failed to add stocks to player");
				Py_XDECREF(pStocks);
				Py_XDECREF(pStocksKey);
				Py_DECREF(pGsDict);
				return NULL;
			}
			
			Py_DECREF(pStocksKey);
			Py_DECREF(pStocks);
			
			
			//Tiles- just like stocks
			//FIXME: Error checking
			PyObject* pTiles = PyList_New(NUM_CHAINS);
			PyObject* pTilesKey = PyBytes_FromString("tiles");
			for(int j = 0; j < HAND_SIZE; j++)
			{
				PyObject* pTile = PyLong_FromLong(gs->players[i].tiles[j]);
				PyList_SET_ITEM(pTiles, j, pTile);
				Py_XDECREF(pTile);
			}
			//Clean up references
			if( !pTiles || !pTilesKey || !PyDict_SetItem(pGsDict, pTilesKey, pTiles))
			{
				PRINT_MSG("Failed to add tiles to player");
				Py_XDECREF(pTiles);
				Py_XDECREF(pTilesKey);
				Py_DECREF(pGsDict);
				return NULL;
			}
			
			Py_DECREF(pTilesKey);
			Py_DECREF(pTiles);
			
			
			//NOTE: Do NOT copy actions! That seems like a pain to implement.
			//If you want this... do it yourself.
			
			
			//Player name
			//FIXME: Error checking
			PyObject* pName = PyBytes_FromString(gs->players[i].name);
			PyObject* pNameKey = PyBytes_FromString("name");
			PyDict_SetItem(pPDict, pNameKey, pName);
			Py_DECREF(pName);
			Py_DECREF(pNameKey);
			
			
			
			//Add the new player's dict to the list
			PyList_SET_ITEM(pPlayers, i, pPDict);
			Py_DECREF(pPDict);
		}
		
		
		//Add players to the dict
		//FIXME: Names
		if( !pPlayers || !pKey || !PyDict_SetItem(pGsDict, pKey, pPlayers))
		{
			PRINT_MSG("Failed to add player to dict");
			Py_XDECREF(pPlayers);
			Py_XDECREF(pKey);
			Py_DECREF(pGsDict);
			return NULL;
		}
		
		Py_DECREF(pKey);
		Py_DECREF(pPlayers);
	}
	
	return pGsDict;
}

#endif //HALE_ENABLE_PYTHON
