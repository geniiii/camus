#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <chip8.h>
#include <gui.h>
#include <nuklear/nuklear_sdl_opengl3.h>

#define TIMER_SPEED 60
#define SPEED		(1000 / TIMER_SPEED)
#define IPS_MULT	9

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

		while (acc >= SPEED) {
			acc -= SPEED;

			if (c.cpu.halt) {
				continue;
			}

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
				case SDL_QUIT: c.running = false; break;
				case SDL_KEYUP: {
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						/* Draw the CHIP8 screen if the GUI is being closed and the delay timer is above 0
						   If the delay timer is above 0, the CHIP8 screen won't be drawn until it gets down to 0 and a DXYN opcode is executed
						   This results in the GUI still being on the screen, despite being closed */
						if (gui.open && c.delay > 0) {
							chip8_screen_draw(&c.screen);
						}

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
