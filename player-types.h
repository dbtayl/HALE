#ifndef HALE_PLAYER_TYPES_H
#define HALE_PLAYER_TYPES_H

#define NUM_PLAYER_TYPES (11)

//Because dealing with dynamic libraries seems like a huge pain, just
//manually include all player/AI types here
#include "ai-bad.h"
#include "ai-dan.h"
#include "ai-greedy.h"
#include "ai-human.h"
#include "ai-python-sample.h"
#include "ai-random.h"
#include "ai-python-andy.h"
#include "ai-python-jim.h"
#include "ai-python-charlie.h"
#include "ai-python-james.h"
#include "ai-python-mike.h"

extern const PlayerActions_t* PlayerTypes[];


#endif
