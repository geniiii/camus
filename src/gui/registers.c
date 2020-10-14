#include <gui/registers.h>

#define NAME "Registers"

bool camus_gui_registers(camus_gui_t* g) {
	/* NUM_REGS (16) + PC (1) + SP (1) */
	if (nk_begin(g->ctx, NAME, nk_rect(50, 20, 125, (NUM_REGS + 1 + 1) * 21),
				 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
		/* PC */
		nk_layout_row_begin(g->ctx, NK_DYNAMIC, 14, 2);
		{
			nk_layout_row_push(g->ctx, 0.5f);
			nk_label(g->ctx, "PC:", NK_TEXT_LEFT);

			nk_layout_row_push(g->ctx, 0.75f);
			char pc[4 + 1];
			sprintf(pc, "0x%X", g->c->cpu.pc);
			nk_label(g->ctx, pc, NK_TEXT_RIGHT);
		}
		nk_layout_row_end(g->ctx);

		/* SP */
		nk_layout_row_begin(g->ctx, NK_DYNAMIC, 14, 2);
		{
			nk_layout_row_push(g->ctx, 0.5f);
			nk_label(g->ctx, "SP:", NK_TEXT_LEFT);

			nk_layout_row_push(g->ctx, 0.75f);
			char sp[2 + 1];
			sprintf(sp, "0x%X", g->c->cpu.sp);
			nk_label(g->ctx, sp, NK_TEXT_RIGHT);
		}
		nk_layout_row_end(g->ctx);

		/* Registers */
		for (u8 i = 0; i < NUM_REGS; ++i) {
			nk_layout_row_begin(g->ctx, NK_DYNAMIC, 14, 2);
			{
				nk_layout_row_push(g->ctx, 0.5f);
				char reg[3 + 1];
				sprintf(reg, "V%d:", i);
				nk_label(g->ctx, reg, NK_TEXT_LEFT);

				nk_layout_row_push(g->ctx, 0.75f);
				char v[2 + 1];
				sprintf(v, "0x%X", g->c->cpu.v[i]);
				nk_label(g->ctx, v, NK_TEXT_RIGHT);
			}
			nk_layout_row_end(g->ctx);
		}
	}
	nk_end(g->ctx);

	return !(nk_window_is_hidden(g->ctx, NAME) || nk_window_is_closed(g->ctx, NAME));
}