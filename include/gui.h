#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#include <nuklear/nuklear_config.h>
#include <nuklear.h>

#include <chip8/chip8.h>

typedef struct camus_gui {
	chip8_t*		c;
	chip8_screen_t* s;

	struct nk_context* ctx;

	bool open;
} camus_gui_t;

extern u8 camus_gui_init(camus_gui_t* g, chip8_t* c);

extern u8 camus_gui_update(camus_gui_t* g);
extern u8 camus_gui_draw(camus_gui_t* g);

extern u8 camus_gui_destroy(camus_gui_t* g);

#endif