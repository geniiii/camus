#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdbool.h>

#include <util/types.h>

#define ON_COLOR  0xFFFFFFFF
#define OFF_COLOR 0xFF000000

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32
#define SCREEN_SIZE	  (SCREEN_WIDTH * SCREEN_HEIGHT)

struct chip8_screen {
	GLuint texture;
	GLuint fbo;

	SDL_Window*	  window;
	SDL_GLContext gl_ctx;

	u32 pixels[SCREEN_SIZE];
};

extern u8 chip8_screen_init(struct chip8_screen* s);

extern u8	chip8_screen_draw(struct chip8_screen* s);
extern void chip8_screen_clear(struct chip8_screen* s);

extern void chip8_screen_destroy(struct chip8_screen* s);

#endif
