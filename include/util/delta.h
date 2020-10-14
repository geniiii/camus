#ifndef DELTA_H
#define DELTA_H

#include <util/types.h>

typedef struct camus_delta {
	u32 tick;
	u32 last_tick;
	u32 accumulator;
	u32 delta;
} camus_delta_t;

#endif