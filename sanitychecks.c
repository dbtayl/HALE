#include "state.h"
#include "board.h"
#include "config.h"

#if (BOARD_TILES < (HAND_SIZE * MAX_PLAYERS + MAX_PLAYERS))
#error Not enough tiles on the board to deal starting hand (BOARD_TILES too small, or MAX_PLAYERS * HAND_SIZE too large)
#endif

#if (BOARD_WIDTH != 12)
#error Game has lookup table assuming a 12x9 board- width is wrong
#endif

#if (BOARD_HEIGHT != 9)
#error Game has lookup table assuming a 12x9 board- width is wrong
#endif
