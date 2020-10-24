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

typedef struct chip8_screen {
	GLuint texture;
	GLuint fbo;

	SDL_Window*	  window;
	SDL_GLContext gl_ctx;

	u32 pixels[SCREEN_SIZE];

	bool draw;
} chip8_screen_t;

extern u8 chip8_screen_init(chip8_screen_t* s);

extern u8	chip8_screen_draw(chip8_screen_t* s);
extern void chip8_screen_clear(chip8_screen_t* s);

extern void chip8_screen_destroy(chip8_screen_t* s);

#endif
