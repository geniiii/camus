#include <util/delta.h>

#include <SDL2/SDL.h>

void camus_delta_init(camus_delta_t* d) {
	d->curr = SDL_GetPerformanceCounter();
	d->last = 0;
}

double camus_delta_get(camus_delta_t* d) {
	d->last = d->curr;
	d->curr = SDL_GetPerformanceCounter();

	return (d->curr - d->last) * 1000 / (double) SDL_GetPerformanceFrequency();
}
