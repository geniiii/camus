#include <gui/registers.h>

#define NAME "Registers"

bool camus_gui_registers(camus_gui_t* g) {
	if (nk_begin(g->ctx, NAME, nk_rect(50, 50, 125, NUM_REGS * 21),
				 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
		for (u8 i = 0; i < NUM_REGS; ++i) {
			nk_layout_row_begin(g->ctx, NK_DYNAMIC, 14, 2);
			{
				nk_layout_row_push(g->ctx, 0.5f);
				char reg[3 + 1];
				sprintf(reg, "V%d:", i);
				nk_label(g->ctx, reg, NK_TEXT_LEFT);

				nk_layout_row_push(g->ctx, 0.75f);
				char v[3 + 1];
				itoa(g->c->cpu.v[i], v, 10);
				nk_label(g->ctx, v, NK_TEXT_RIGHT);
			}
			nk_layout_row_end(g->ctx);
		}
	}
	nk_end(g->ctx);

	return !(nk_window_is_hidden(g->ctx, NAME) || nk_window_is_closed(g->ctx, NAME));
}