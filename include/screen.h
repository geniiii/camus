#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "types.h"

#define ON_COLOR  0xFFFFFFFF
#define OFF_COLOR 0xFF000000

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32
#define SCREEN_SIZE	  (SCREEN_WIDTH * SCREEN_HEIGHT)

typedef struct chip8_screen_s {
	SDL_Texture*  texture;
	SDL_Window*	  window;
	SDL_Renderer* renderer;

	bool draw;
} chip8_screen_t;

extern u8 chip8_screen_init(chip8_screen_t* s);

extern u8	chip8_screen_draw(chip8_screen_t* s);
extern void chip8_screen_clear(chip8_screen_t* s);

extern void chip8_screen_destroy(chip8_screen_t* s);

#endif