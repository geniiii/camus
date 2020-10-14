#include <gui.h>

#include <chip8/cpu.h>
#include <SDL2/SDL_opengl.h>
#include <nuklear/nuklear_sdl_opengl3.h>
#include <gui/menubar.h>
#include <gui/registers.h>

#define MAX_VERTEX_MEMORY  512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define UPDATE_PANEL(panel, function) \
	if (g->panels & panel) {          \
		if (!function(g)) {           \
			g->panels &= ~panel;      \
		}                             \
	}

void camus_gui_init(camus_gui_t* g, chip8_t* c) {
	g->c = c;
	g->s = &c->screen;

	g->open = false;

	/* Default panels
	   The menubar is never disabled. */
	g->panels |= MENUBAR | REGISTERS;

	g->ctx = nk_sdl_init(g->s->window);

	{
		struct nk_font_atlas* atlas;
		nk_sdl_font_stash_begin(&atlas);
		nk_sdl_font_stash_end();
	}

	/* Fuck rounded UI elements. */
	g->ctx->style.button.rounding			 = 0;
	g->ctx->style.menu_button.rounding		 = 0;
	g->ctx->style.contextual_button.rounding = 0;
}

void camus_gui_update(camus_gui_t* g) {
	UPDATE_PANEL(MENUBAR, camus_gui_menubar)
	UPDATE_PANEL(REGISTERS, camus_gui_registers)
}

void camus_gui_draw(camus_gui_t* g) {
	u32 win_width, win_height;

	SDL_GetWindowSize(g->s->window, &win_width, &win_height);
	glViewport(0, 0, win_width, win_height);

	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

void camus_gui_destroy(camus_gui_t* g) {
	nk_sdl_shutdown();
}