#ifndef DELTA_H
#define DELTA_H

#include <util/types.h>

struct camus_delta {
	double curr;
	double last;
};

extern void camus_delta_init(struct camus_delta* restrict d);

extern double camus_delta_get(struct camus_delta* restrict d);

#endif
