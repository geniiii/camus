#include <gui.h>

#include <SDL2/SDL_opengl.h>
#include <nuklear/nuklear_sdl_opengl3.h>

#define MAX_VERTEX_MEMORY  512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

u8 camus_gui_init(camus_gui_t* g, chip8_screen_t* s) {
	g->screen = s;

	g->open = false;

	g->ctx = nk_sdl_init(g->screen->window);

	{
		struct nk_font_atlas* atlas;
		nk_sdl_font_stash_begin(&atlas);
		nk_sdl_font_stash_end();
	}
}

u8 camus_gui_update(camus_gui_t* g) {
}

u8 camus_gui_draw(camus_gui_t* g) {
	u32 win_width, win_height;

	SDL_GetWindowSize(g->screen->window, &win_width, &win_height);
	glViewport(0, 0, win_width, win_height);

	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

u8 camus_gui_destroy(camus_gui_t* g) {
	nk_sdl_shutdown();
}