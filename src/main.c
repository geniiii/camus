#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include "chip8.h"

int main(int argc, char** argv) {
	chip8_t c;
	chip8_init(&c);

	chip8_load(&c, argv[1]);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		puts("Failed to init SDL");
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("camus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 12, SCREEN_HEIGHT * 12, SDL_WINDOW_SHOWN);
	if (!window) {
		puts("Failed to create window");
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture*  texture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
											   SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	uint32_t  start_tick;
	uint32_t  frame_speed;
	SDL_Event event;
	bool	  running = true;
	while (running) {
		start_tick = SDL_GetTicks();

		chip8_emulate_cycle(&c);
		if (c.draw) {
			SDL_UpdateTexture(texture, NULL, c.screen,
							  SCREEN_WIDTH * sizeof(u32));
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);

			c.draw = false;
		}

		frame_speed = SDL_GetTicks() - start_tick;
		if (frame_speed < 1000 / 60) {
			SDL_Delay(1000 / 60 - frame_speed);
		}

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
	}

	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}
