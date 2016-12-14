#include "state.h"
#include "board.h"
#include "config.h"

#if (BOARD_TILES < (HAND_SIZE * MAX_PLAYERS + MAX_PLAYERS))
#error Not enough tiles on the board to deal starting hand (BOARD_TILES too small, or MAX_PLAYERS * HAND_SIZE too large)
#endif
