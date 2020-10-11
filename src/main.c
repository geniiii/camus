#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include "chip8.h"

#define SPEED	 (1000 / 60)
#define IPS_MULT 9

int main(int argc, char** argv) {
	u8 error = 0;

	chip8_t c;
	if ((error = chip8_init(&c)) > 0) {
		goto exit;
	}
	if ((error = chip8_load(&c, argv[1]) > 0)) {
		goto exit;
	}

	u32		  tick;
	u32		  last_tick	  = 0;
	u32		  accumulator = 0;
	u32		  delta;
	SDL_Event event;
	bool	  running = true;
	while (running) {
		tick	  = SDL_GetTicks();
		delta	  = tick - last_tick;
		last_tick = tick;
		accumulator += delta;

		while (accumulator >= SPEED) {
			/* 540Hz cycle loop */
			for (u8 cycles = 1; cycles <= IPS_MULT; ++cycles) {
				chip8_emulate_cycle(&c);
				if (c.screen.draw) {
					chip8_screen_draw(&c.screen);
					c.screen.draw = false;
				}
			}

			if (c.delay) {
				c.delay--;
			}
			if (c.sound) {
				c.sound--;
			}

			accumulator -= SPEED;
		}

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
	}

exit:
	chip8_screen_destroy(&c.screen);

	SDL_Quit();

	return error;
}
