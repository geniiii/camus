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

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
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
				case SDL_DROPFILE: {
					char* out_path = event.drop.file;
					chip8_reset(&c);

					chip8_load(&c, out_path);
					SDL_free(out_path);

					chip8_screen_clear(&c.screen);
				}
			}

			if (gui.open) {
				nk_sdl_handle_event(&event);
			}
		}
		nk_input_end(gui.ctx);

		chip8_screen_draw(&c.screen);
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
