#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#include <nuklear/nuklear_config.h>
#include <nuklear.h>

#include <chip8.h>

#define PANEL(id) (1u << id)
typedef enum camus_gui_open {
	MENUBAR	  = PANEL(0),
	REGISTERS = PANEL(1),
	SPEED	  = PANEL(2),
	ABOUT	  = PANEL(3)
} camus_gui_open_t;
typedef u32 camus_gui_flags;

typedef struct camus_gui {
	chip8_t*		c;
	chip8_screen_t* s;

	struct nk_context* ctx;

	bool open;

	camus_gui_flags panels;
} camus_gui_t;

extern void camus_gui_init(camus_gui_t* g, chip8_t* c);

extern void camus_gui_update(camus_gui_t* g);
extern void camus_gui_draw(camus_gui_t* g);

extern void camus_gui_destroy(camus_gui_t* g);

#undef PANEL

#endif