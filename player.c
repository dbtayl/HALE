#include "config.h"
#include "player.h"
#include "state.h"
#include "util.h"

HALE_status_t giveTile(GameState_t* gs, uint8_t tile, uint8_t playerNum)
{
	CHECK_NULL_PTR(gs, "gs");
	
	if(playerNum >= gs->numPlayers)
	{
		PRINT_MSG("playerNum out of bounds");
		return HALE_OOB;
	}
	
	int i;
	for(i = 0; i < HAND_SIZE; i++)
	{
		if(gs->players[playerNum].tiles[i] == TILE_NULL)
		{
			gs->players[playerNum].tiles[i] = tile;
			return HALE_OK;
		}
	}
	
	return HALE_PLAYER_HAND_FULL;
}



HALE_status_t calculatePlayerValue(GameState_t* gs, uint8_t playerNum, int32_t* val)
{
	CHECK_NULL_PTR(gs, "gs");
	CHECK_NULL_PTR(val, "val");
	
	if(playerNum >= gs->numPlayers)
	{
		PRINT_MSG("playerNum out of bounds");
		return HALE_OOB;
	}
	
	Player_t* p = &(gs->players[playerNum]);
	*val = p->cash;
	
	PRINT_MSG("FIXME: Need to finish calculating the player's value");
	HANDLE_UNRECOVERABLE_ERROR(HALE_FUNC_NOT_IMPLEMENTED);

	return HALE_OK;
}
