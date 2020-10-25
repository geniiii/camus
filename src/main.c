#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <chip8.h>
#include <gui.h>
#include <nuklear/nuklear_sdl_opengl3.h>

int main(int argc, char** argv) {
	u8 error = 0;

	struct chip8 c;
	chip8_init(&c);
	if ((error = chip8_screen_init(&c.screen)) > 0) {
		goto exit;
	}
	if (argc > 1) {
		if ((error = chip8_load(&c, argv[1]) > 0)) {
			goto exit;
		}
	} else {
		c.cpu.halt = true;
	}

	struct camus_gui gui;
	camus_gui_init(&gui, &c);

	SDL_Event event;

	double acc = 0;
	while (c.running) {
		acc += camus_delta_get(&c.delta);

		while (acc >= c.cpu.speed.timer_speed) {
			acc -= c.cpu.speed.timer_speed;

			if (c.cpu.halt) {
				continue;
			}

			/* Opcode loop, by default 540Hz (9 instructions per each 60Hz timer update) */
			for (u8 cycles = 0; cycles < c.cpu.speed.ips_mult; ++cycles) {
				chip8_emulate_cycle(&c);
				if (c.screen.draw) {
					chip8_screen_draw(&c.screen);
					c.screen.draw = false;
				}
			}

			if (c.delay > 0) {
				c.delay--;
			}
			if (c.sound > 0) {
				c.sound--;
			}
		}

		nk_input_begin(gui.ctx);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: c.running = false; break;
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

		/* Draw the CHIP8 screen if it hasn't already been drawn to avoid GUI ghosting */
		if (c.delay > 0 || c.cpu.halt || (!gui.open && !c.screen.draw)) {
			chip8_screen_draw(&c.screen);
		}

		if (gui.open) {
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
