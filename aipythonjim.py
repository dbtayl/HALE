#NOTE: Hopefully you'll be able to do something like "import hale" to get
#access to things like NUM_CHAINS, NUM_TILES, etc.

#Return a tile (that exists in the player's hand) to play
#Should be a number from 0-(BOARD_TILES-1)
def pythonJimPlayTile(gs, playerNum):
	#FIXME: Return some oddball value we can recognize when debugging
	print("In pythonJimPlayTile! FINISH ME!")
	return 17

#Return a chain_t (integer 0 <= n < NUM_CHAINS)
#Should be a valid chain to form
def pythonJimFormChain(gs, playerNum):
	#FIXME: Return some oddball value we can recognize when debugging
	print("In pythonJimFormChain! FINISH ME!")
	return 31

#Returns a chain_t (integer 0 <= n < NUM_CHAINS) from the array of options
def pythonJimMergerSurvivor(gs, playerNum, options):
	#FIXME: Return some oddball value we can recognize when debugging
	print("In pythonJimMergerSurvivor! FINISH ME!")
	return 5

#Returns an array of order to merge chains in. See player.h.
#Return array size should be NUM_CHAINS
def pythonJimMergerOrder(gs, playerNum, survivor, options):
	#FIXME: Return some oddball value we can recognize when debugging
	print("In pythonJimMergerOrder! FINISH ME!")
	return [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF]

#Return list of length NUM_CHAINS, indicating which stocks to buy
def pythonJimBuyStock(gs, playerNum):
	#FIXME: Return some oddball value we can recognize when debugging
	print("In pythonJimBuyStock! FINISH ME!")
	return [0, 1, 2, 4, 8, 16, 32]

#Return (tradefor, sell) tuple, one number each
def pythonJimMergerTrade(gs, playerNum, survivor, defunct):
	#FIXME: Return some oddball value we can recognize when debugging
	print("In pythonJimMergerTrade! FINISH ME!")
	return (2, 3)

#Return essentially true/false, or 0/<non-zero>
def pythonJimEndGame(gs, playerNum):
	#FIXME: Return some oddball value we can recognize when debugging
	print("In pythonJimEndGame! FINISH ME!")
	return 101
