#include <util/delta.h>

#include <SDL2/SDL.h>

void camus_delta_init(struct camus_delta* restrict d) {
	d->curr = SDL_GetPerformanceCounter();
	d->last = 0;
}

double camus_delta_get(struct camus_delta* restrict d) {
	d->last = d->curr;
	d->curr = SDL_GetPerformanceCounter();

	return (d->curr - d->last) * 1000 / (double) SDL_GetPerformanceFrequency();
}
