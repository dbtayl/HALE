#include <stdio.h>
#include <stdlib.h>

#include "util.h"

const char* err_msg[HALE_NUM_STATUS] = {"OK", "FAIL", "TILE_POOL_EMPTY", "TILE_POOL_CORRUPTED", "PLAYER_HAND_FULL", "OOB", "BAD_INPUT", "FUNC_NOT_IMPLEMENTED"};

void handleUnrecoverableError(const char* func, HALE_status_t err_code)
{
	printf("FATAL: %s: %s\n", func, err_msg[err_code]);
	exit(err_code);
}


uint8_t getRandom8(uint16_t min, uint16_t max)
{
#ifndef GO_FAST_AND_BREAK_THINGS
	if( (min & 0x00ff) )
	{
		PRINT_MSG_INT("min OOB", min);
		HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
	}
	if( (max & 0x00ff) )
	{
		PRINT_MSG_INT("max OOB", max);
		HANDLE_UNRECOVERABLE_ERROR(HALE_OOB);
	}
#endif //GO_FAST_AND_BREAK_THINGS
	
	uint16_t d = (max - min) + 1;
	return (uint8_t)(rand() % d + min);
}
