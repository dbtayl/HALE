#Constants- probably shouldn't just be copied from C code, but so be it
#How much money each player starts with
STARTING_CASH = (6000)

#How many tiles are in a player's hand
HAND_SIZE = (6)

#How many units of stock are available for each chain
NUM_STOCKS = (25)

#Maximum number of players
MAX_PLAYERS = (4)

#How many chains exist
NUM_CHAINS = (7)

#How big a chain has to be before it cannot disappear
SAFE_CHAIN_SIZE = (11)

#How large a chain must be to qualify for ending the game
CHAIN_SIZE_GAME_END = (41)

#Size of the board
BOARD_WIDTH = (12)
BOARD_HEIGHT = (9)

#Shortcut for when you need the total size of the board
BOARD_TILES = (BOARD_WIDTH * BOARD_HEIGHT)

#Specifies an invalid board index (eg, left of the left column)
BOARD_NULL = (0xff)

#Null tile represents either an empty place on the board or an empty
#slot in a player's hand
TILE_NULL = (0xff)


#How many shares you can buy per turn
SHARES_PER_TURN = (3)

#Names of each hotel chain, in order
CHAIN_LUXOR = 0
CHAIN_TOWER = 1
CHAIN_WORLDWIDE = 2
CHAIN_AMERICAN = 3
CHAIN_FESTIVAL = 4
CHAIN_IMPERIAL = 5
CHAIN_CONTINENTAL = 6
CHAIN_NONE = 7
chainNames = ["Luxor", "Tower", "Worldwide", "American", "Festival", "Imperial", "Continental", "None"]


#Lookup table for adjacent tile indices
squareAdjacencies = [
[BOARD_NULL, BOARD_NULL, 1, 12],
[BOARD_NULL, 0, 2, 13],
[BOARD_NULL, 1, 3, 14],
[BOARD_NULL, 2, 4, 15],
[BOARD_NULL, 3, 5, 16],
[BOARD_NULL, 4, 6, 17],
[BOARD_NULL, 5, 7, 18],
[BOARD_NULL, 6, 8, 19],
[BOARD_NULL, 7, 9, 20],
[BOARD_NULL, 8, 10, 21],
[BOARD_NULL, 9, 11, 22],
[BOARD_NULL, 10, BOARD_NULL, 23],
[0, BOARD_NULL, 13, 24],
[1, 12, 14, 25],
[2, 13, 15, 26],
[3, 14, 16, 27],
[4, 15, 17, 28],
[5, 16, 18, 29],
[6, 17, 19, 30],
[7, 18, 20, 31],
[8, 19, 21, 32],
[9, 20, 22, 33],
[10, 21, 23, 34],
[11, 22, BOARD_NULL, 35],
[12, BOARD_NULL, 25, 36],
[13, 24, 26, 37],
[14, 25, 27, 38],
[15, 26, 28, 39],
[16, 27, 29, 40],
[17, 28, 30, 41],
[18, 29, 31, 42],
[19, 30, 32, 43],
[20, 31, 33, 44],
[21, 32, 34, 45],
[22, 33, 35, 46],
[23, 34, BOARD_NULL, 47],
[24, BOARD_NULL, 37, 48],
[25, 36, 38, 49],
[26, 37, 39, 50],
[27, 38, 40, 51],
[28, 39, 41, 52],
[29, 40, 42, 53],
[30, 41, 43, 54],
[31, 42, 44, 55],
[32, 43, 45, 56],
[33, 44, 46, 57],
[34, 45, 47, 58],
[35, 46, BOARD_NULL, 59],
[36, BOARD_NULL, 49, 60],
[37, 48, 50, 61],
[38, 49, 51, 62],
[39, 50, 52, 63],
[40, 51, 53, 64],
[41, 52, 54, 65],
[42, 53, 55, 66],
[43, 54, 56, 67],
[44, 55, 57, 68],
[45, 56, 58, 69],
[46, 57, 59, 70],
[47, 58, BOARD_NULL, 71],
[48, BOARD_NULL, 61, 72],
[49, 60, 62, 73],
[50, 61, 63, 74],
[51, 62, 64, 75],
[52, 63, 65, 76],
[53, 64, 66, 77],
[54, 65, 67, 78],
[55, 66, 68, 79],
[56, 67, 69, 80],
[57, 68, 70, 81],
[58, 69, 71, 82],
[59, 70, BOARD_NULL, 83],
[60, BOARD_NULL, 73, 84],
[61, 72, 74, 85],
[62, 73, 75, 86],
[63, 74, 76, 87],
[64, 75, 77, 88],
[65, 76, 78, 89],
[66, 77, 79, 90],
[67, 78, 80, 91],
[68, 79, 81, 92],
[69, 80, 82, 93],
[70, 81, 83, 94],
[71, 82, BOARD_NULL, 95],
[72, BOARD_NULL, 85, 96],
[73, 84, 86, 97],
[74, 85, 87, 98],
[75, 86, 88, 99],
[76, 87, 89, 100],
[77, 88, 90, 101],
[78, 89, 91, 102],
[79, 90, 92, 103],
[80, 91, 93, 104],
[81, 92, 94, 105],
[82, 93, 95, 106],
[83, 94, BOARD_NULL, 107],
[84, BOARD_NULL, 97, BOARD_NULL],
[85, 96, 98, BOARD_NULL],
[86, 97, 99, BOARD_NULL],
[87, 98, 100, BOARD_NULL],
[88, 99, 101, BOARD_NULL],
[89, 100, 102, BOARD_NULL],
[90, 101, 103, BOARD_NULL],
[91, 102, 104, BOARD_NULL],
[92, 103, 105, BOARD_NULL],
[93, 104, 106, BOARD_NULL],
[94, 105, 107, BOARD_NULL],
[95, 106, BOARD_NULL, BOARD_NULL]
]



#These are convenience re-implementations of C functions. They've not
#been any sort of tested.

#Also note that the implementations are slightly different than in C-
#some minor behavior differences!


#Returns the values of the squares up, left, right,
#and down from the tile <tile>.
def getAdjacentSquares(gs, tile):
	if(not gs):
		print(getAdjacentSquares.__name__ + ": gs is None")
		return None
	
	if(tile >= BOARD_TILES):
		print(getAdjacentSquares.__name__ + ": tile is invalid: " + str(tile))
		return None
	
	#Turns out this is a really simple function in Python...
	
	adj = squareAdjacencies[tile]
	out = [0 for i in range(4)]
	
	for i in range(4):
		if(adj[a] == BOARD_NULL):
			out[i] = TILE_NULL
		else:
			out[i] = gs["board"][adj[i]]
	
	return out


#Python re-implementation of the C function by the same name
#Returns a list of chains that would merge, possibly empty (if no merger)
def wouldCauseMerger(gs, tile):
	#NOTE: Don't bother checking if the tile has already been played-
	#the return value should be valid regardless
	if(not gs):
		print(wouldCauseMerger.__name__ + ": gs is None")
		return None
	
	if(tile >= BOARD_TILES):
		print(wouldCauseMerger.__name__ + ": tile is invalid: " + str(tile))
		return None
	
	#Get adjacent tiles
	adj = getAdjacentSquares(gs, tile)
	
	#Remove NULL and empty chains
	adj = [x for x in adj if x < NUM_CHAINS]
	
	#Remove duplicates
	adj = list(set(adj))
	
	#If there's only one chain left... remove it to, since that's not a merger
	if(len(adj) <= 1):
		return []
	
	#Otherwise, the list should have all the merging chains at this point
	return adj


#Python re-implementation of the C function by the same name
#Returns true or false
def wouldCreateChain(gs, tile):
	if(not gs):
		print(wouldCauseMerger.__name__ + ": gs is None")
		return None
	
	if(tile >= BOARD_TILES):
		print(wouldCauseMerger.__name__ + ": tile is invalid: " + str(tile))
		return None
		
	#Get adjacent tiles
	adj = getAdjacentSquares(gs, tile)
	
	#Adjacent chain? Definitely not creating a new one
	chainTiles = [x for x in adj if(x < NUM_CHAINS)]
	if(chainTiles):
		return False
	
	#No adjacent chain AND a non-chain tile adjacent? New chain!
	nonChainTiles = [x for x in adj if(x == CHAIN_NONE)]
	if(nonChainTiles):
		return True
	
	#Absolutely nothing around? No new chain.
	return False


#Python re-implementation of the C function by the same name
#Really simple- just count the number of tiles on the board for each chain
def getChainSizes(gs):
	sizes = [0 for x in range(NUM_CHAINS)]
	
	if(not gs):
		print(getChainSizes.__name__ + ": gs is None")
		return None
	
	for i in range(BOARD_TILES):
		if(gs["board"][i] < NUM_CHAINS):
			sizes[gs["board"][i]] += 1
	return sizes


#Python re-implementation of the C function by the same name
#Returns a list containing the price for one share in each of the chains
def getChainPricesPerShare(gs):
	prices = [0 for x in range(NUM_CHAINS)]
	if(not gs):
		print(getChainPrices.__name__ + ": gs is None")
		return None
	
	sizes = getChainSizes(gs)
	
	#Cycle through the chains and calculate prices
	for i in range(NUM_CHAINS):
		if(sizes[i] < 6):
			prices[i] = sizes[i]*100
		elif(sizes[i] > 40):
			prices[i] = 1000
		#sizes[i] somewhere betwen 6 and 40, inclusive
		else:
			prices[i] = 600 + ((sizes[i]-1)/10) * 100
		
		#Add "bonus cost" for more expensive chains
		#+$100 for mid-tier
		if(i > CHAIN_TOWER):
			prices[i] += 100
		#ANOTHER +$100 for top-tier
		if(i > CHAIN_FESTIVAL):
			prices[i] += 100
	
	return prices


#Python re-implementation of the C function by the same name
#Returns true if a tile is valid to play, false if not
def isValidTilePlay(gs, tile):
	#Not valid if the game state is invalid
	if(gs is None):
		print("isValidTilePlay: gs is None")
		return False
	
	#Firstly, if the TILE isn't valid, it can't be a valid play
	#These are arguably redundant checks, but it seems like a good
	#practice to have both of them in there
	#The last check really shouldn't be necessary, but it's also technically
	#an invalid more to play a tile that's already in play, so...
	if( (tile >= BOARD_TILES) or (tile == TILE_NULL) or gs["board"][tile] != CHAIN_EMPTY):
		print(isValidTilePlay.__name__ + ": tile OOB... or something like that. tile was: " + str(tile))
		return False
	
	mergingChains = wouldCauseMerger(gs, tile)
	create = wouldCreateChain(gs, tile)
	
	#If it doesn't cause a merger or create a chain, it must be OK
	if( (not mergingChains) and (not create) ):
		print(isValidTilePlay.__name__ + ": tile valid: " + str(tile))
		return True
	
	
	#Otherwise, we need to know how big the chains are
	sizes = getChainSizes(gs)
	
	
	#If this would cause a merger, make sure the merger is legal
	if(mergingChains):
		#Can only be illegal if trying to merge two or more safe chains
		safeCount = 0
		
		#Look through chains that are merging for safe chains
		for c in mergingChains:
			#If this chain is merging AND it's safe, record it
			if( sizes[c] >= SAFE_CHAIN_SIZE ):
				safeCount += 1
		
		#If two or more are safe, it's an illegal move
		if(safeCount > 1):
			print(isValidTilePlay.__name__ + ": tile would merge two safe chains. tile: " + str(tile))
			return False
		
		print(isValidTilePlay.__name__ + ": tile would cause valid merger: " + str(tile))
	
	
	#If this would create a chain, make sure creating a chain is legal
	if(create):
		legal = False
		
		#Check if any chain is of size 0
		for i in range(NUM_CHAINS):
			if(sizes[i] == 0):
				legal = True
				break

		#if not, return invalid move
		if(not legal):
			print(isValidTilePlay.__name__ + ": tile would create a new chain, but no new chains available. tile: " + str(tile))
			return False
			
	
	#If it's not illegal, it must be legal
	return True


