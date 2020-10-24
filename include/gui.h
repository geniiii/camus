#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#include <nuklear/nuklear_config.h>
#include <nuklear.h>

#include <chip8.h>

#define PANEL(id) (1u << id)
enum camus_gui_open {
	MENUBAR	  = PANEL(0),
	REGISTERS = PANEL(1),
	SPEED	  = PANEL(2),
	ABOUT	  = PANEL(3),
	MEMORY	  = PANEL(4)
};
typedef u32 camus_gui_flags;

struct camus_gui {
	struct chip8*		 c;
	struct chip8_screen* s;

	struct nk_context* ctx;

	bool open;

	camus_gui_flags panels;
};

extern void camus_gui_init(struct camus_gui* restrict g, struct chip8* restrict c);

extern void camus_gui_update(struct camus_gui* restrict g);
extern void camus_gui_draw(struct camus_gui* restrict g);

extern void camus_gui_destroy(struct camus_gui* restrict g);

#undef PANEL

#endif
