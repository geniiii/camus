#ifndef DELTA_H
#define DELTA_H

#include <util/types.h>

typedef struct camus_delta {
	double curr;
	double last;
} camus_delta_t;

extern void camus_delta_init(camus_delta_t* d);

extern double camus_delta_get(camus_delta_t* d);

#endif
