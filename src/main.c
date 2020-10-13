#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "chip8/chip8.h"
#include "gui.h"
#include "nuklear/nuklear_sdl_opengl3.h"

#define TIMER_SPEED 60
#define SPEED		(1000 / TIMER_SPEED)
#define IPS_MULT	9

int main(int argc, char** argv) {
	u8 error = 0;

	chip8_t c;
	if ((error = chip8_init(&c)) > 0) {
		goto exit;
	}
	if ((error = chip8_load(&c, argv[1]) > 0)) {
		goto exit;
	}

	camus_gui_t gui;
	if ((error = camus_gui_init(&gui, &c)) > 0) {
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
			accumulator -= SPEED;

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
		}

		nk_input_begin(gui.ctx);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: running = false; break;
				case SDL_KEYUP: {
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						gui.open = !gui.open;
					}
					break;
				}
			}

			if (gui.open) {
				nk_sdl_handle_event(&event);
			}
		}
		nk_input_end(gui.ctx);

		if (gui.open) {
			chip8_screen_draw(&c.screen);

			camus_gui_update(&gui);
			camus_gui_draw(&gui);
		}

		SDL_GL_SwapWindow(c.screen.window);
	}

exit:
	camus_gui_destroy(&gui);
	chip8_screen_destroy(&c.screen);

	SDL_Quit();

	return error;
}
