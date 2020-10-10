#include "screen.h"

#include "chip8.h"

u8 chip8_screen_init(chip8_screen_t* s) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fputs("Failed to init SDL (video)", stderr);
		return 1;
	}

	s->window = SDL_CreateWindow("camus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 12, SCREEN_HEIGHT * 12, SDL_WINDOW_SHOWN);
	if (!s->window) {
		fputs("Failed to create window", stderr);
		return 1;
	}

	s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED);
	if (!s->renderer) {
		fputs("Failed to create renderer", stderr);
		return 1;
	}
	s->texture = SDL_CreateTexture(s->renderer, SDL_PIXELFORMAT_ARGB8888,
								   SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!s->renderer) {
		fputs("Failed to create texture", stderr);
		return 1;
	}

	/* Set screen to black initially */
	SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 0);
	SDL_RenderClear(s->renderer);
	SDL_RenderPresent(s->renderer);
}

u8 chip8_screen_draw(chip8_screen_t* s) {
	SDL_RenderClear(s->renderer);
	SDL_RenderCopy(s->renderer, s->texture, NULL, NULL);
	SDL_RenderPresent(s->renderer);
}

void chip8_screen_clear(chip8_screen_t* s) {
	u32* pixels = NULL;
	int	 pitch;

	SDL_LockTexture(s->texture, NULL, (void**) &pixels, &pitch);
	for (u16 i = 0; i < SCREEN_SIZE; ++i) {
		pixels[i] = OFF_COLOR;
	}
	SDL_UnlockTexture(s->texture);

	s->draw = true;
}

void chip8_screen_destroy(chip8_screen_t* s) {
	SDL_DestroyTexture(s->texture);
	s->texture = NULL;
	SDL_DestroyRenderer(s->renderer);
	s->renderer = NULL;
	SDL_DestroyWindow(s->window);
	s->window = NULL;
}
