#include <gui/menubar.h>

#include <nfd.h>

#define PANEL(_enum, name)                                \
	if (nk_menu_item_label(g->ctx, name, NK_TEXT_LEFT)) { \
		if (g->panels & _enum) {                          \
			g->panels &= ~_enum;                          \
		} else {                                          \
			g->panels |= _enum;                           \
		}                                                 \
	}

#define NAME "Menubar"

bool camus_gui_menubar(struct camus_gui* restrict g) {
	u32 win_width;
	SDL_GetWindowSize(g->s->window, &win_width, NULL);
	if (nk_begin(g->ctx, NAME, nk_rect(0, 0, win_width, 20),
				 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND)) {
		nk_menubar_begin(g->ctx);

		nk_layout_row_begin(g->ctx, NK_DYNAMIC, 15, 3);
		{
			nk_layout_row_push(g->ctx, 0.1f);

			if (nk_menu_begin_label(g->ctx, "File", NK_TEXT_LEFT, nk_vec2(120, 200))) {
				nk_layout_row_dynamic(g->ctx, 20, 1);

				if (nk_menu_item_label(g->ctx, "Load", NK_TEXT_LEFT)) {
					nfdchar_t*	out_path = NULL;
					nfdresult_t result	 = NFD_OpenDialog(NULL, NULL, &out_path);
					if (result == NFD_OKAY) {
						chip8_reset(g->c);

						chip8_load(g->c, out_path);
						free(out_path);

						chip8_screen_clear(g->s);
					}
				}
				if (nk_menu_item_label(g->ctx, "Exit", NK_TEXT_LEFT)) {
					g->c->running = false;
				}
				PANEL(ABOUT, "About")

				nk_menu_end(g->ctx);
			}

			if (nk_menu_begin_label(g->ctx, "Debug", NK_TEXT_LEFT, nk_vec2(120, 200))) {
				nk_layout_row_dynamic(g->ctx, 20, 1);
				PANEL(REGISTERS, "Registers")
				nk_menu_end(g->ctx);
			}

			if (nk_menu_begin_label(g->ctx, "Emulation", NK_TEXT_LEFT, nk_vec2(120, 200))) {
				nk_layout_row_dynamic(g->ctx, 20, 1);

				if (nk_menu_item_label(g->ctx, !g->c->cpu.halt ? "Pause" : "Unpause", NK_TEXT_LEFT)) {
					g->c->cpu.halt = !g->c->cpu.halt;
				}
				if (nk_menu_item_label(g->ctx, "Restart", NK_TEXT_LEFT)) {
					chip8_reset(g->c);
					chip8_screen_clear(g->s);
				}

				nk_menu_end(g->ctx);
			}
		}
		nk_menubar_end(g->ctx);
	}
	nk_layout_row_end(g->ctx);
	nk_end(g->ctx);

	return 1;
}
