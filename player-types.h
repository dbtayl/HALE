#ifndef HALE_PLAYER_TYPES_H
#define HALE_PLAYER_TYPES_H

#define NUM_PLAYER_TYPES (5)

//Because dealing with dynamic libraries seems like a huge pain, just
//manually include all player/AI types here
#include "ai-bad.h"
#include "ai-greedy.h"
#include "ai-human.h"
#include "ai-python-sample.h"
#include "ai-random.h"

extern const PlayerActions_t* PlayerTypes[];


#endif
