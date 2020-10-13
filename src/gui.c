#include <gui.h>

#include <chip8/cpu.h>
#include <SDL2/SDL_opengl.h>
#include <nuklear/nuklear_sdl_opengl3.h>

#define MAX_VERTEX_MEMORY  512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

void camus_gui_init(camus_gui_t* g, chip8_t* c) {
	g->c = c;
	g->s = &c->screen;

	g->open = false;

	g->ctx = nk_sdl_init(g->s->window);

	{
		struct nk_font_atlas* atlas;
		nk_sdl_font_stash_begin(&atlas);
		nk_sdl_font_stash_end();
	}
}

void camus_gui_update(camus_gui_t* g) {
	if (nk_begin(g->ctx, "Registers", nk_rect(50, 50, 100, NUM_REGS * 21),
				 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE)) {
		for (u8 i = 0; i < NUM_REGS; ++i) {
			nk_layout_row_begin(g->ctx, NK_DYNAMIC, 14, 2);
			{
				nk_layout_row_push(g->ctx, 0.5f);
				char reg[3 + 1];
				sprintf(reg, "V%d:", i);
				nk_label(g->ctx, reg, NK_TEXT_LEFT);

				nk_layout_row_push(g->ctx, 0.75f);
				char v[3 + 1];
				sprintf(v, "%d", g->c->cpu.v[i]);
				nk_label(g->ctx, v, NK_TEXT_RIGHT);
			}
			nk_layout_row_end(g->ctx);
		}
	}
	nk_end(g->ctx);
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