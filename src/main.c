#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include "chip8.h"

#define SPEED		(1000 / 500)
#define TIMER_SPEED (1000 / 60)

int main(int argc, char** argv) {
	chip8_t c;
	chip8_init(&c);

	chip8_load(&c, argv[1]);

	u32		  start_tick;
	u32		  frame_speed;
	SDL_Event event;
	bool	  running = true;
	while (running) {
		start_tick = SDL_GetTicks();

		chip8_emulate_cycle(&c);
		if (c.screen.draw) {
			chip8_screen_draw(&c.screen);
			c.screen.draw = false;
		}

		frame_speed = SDL_GetTicks() - start_tick;
		if (frame_speed < SPEED) {
			SDL_Delay(SPEED - frame_speed);
		}

		if (c.delay && frame_speed < TIMER_SPEED) {
			c.delay--;
		}
		if (c.sound && frame_speed < TIMER_SPEED) {
			c.sound--;
		}

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
	}

	chip8_screen_destroy(&c.screen);

	SDL_Quit();
}
