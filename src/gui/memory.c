#include <gui/memory.h>

#define NAME "Memory"

bool camus_gui_memory(struct camus_gui* restrict g) {
	if (nk_begin(g->ctx, NAME, nk_rect(50, 50, 580, 600), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_NO_SCROLLBAR)) {
		const struct nk_rect total_space   = nk_window_get_content_region(g->ctx);
		const float			 panel_padding = g->ctx->style.window.padding.x;

		nk_layout_row_begin(g->ctx, NK_STATIC, 10, 0xF + 1);
		{
			nk_layout_row_push(g->ctx, 35 + panel_padding);
			nk_spacing(g->ctx, 1);

			for (u8 i = 0; i <= 0xF; i += 2) {
				char hex[2 + 1 + 1];
				sprintf(hex, "%02X ", i);
				nk_layout_row_push(g->ctx, 60 + panel_padding);
				nk_label(g->ctx, hex, NK_TEXT_ALIGN_LEFT);
			}
		}
		nk_layout_row_end(g->ctx);

		nk_layout_row_static(g->ctx, total_space.h - panel_padding, total_space.w, 1);
		if (nk_group_begin(g->ctx, "Contents", 0)) {
			nk_layout_row_begin(g->ctx, NK_STATIC, 10, 0xF + 1);
			{
				for (u16 i = 0; i < MEM_SIZE; ++i) {
					char hex[4 + 1 + 1];

					if (i % 0x10 == 0) {
						nk_layout_row_push(g->ctx, 35);
						sprintf(hex, "%04X ", i);
					} else {
						nk_layout_row_push(g->ctx, 30);
						sprintf(hex, "%04X ", g->c->mem[i] | g->c->mem[i + 1] << 8);
					}
					nk_label(g->ctx, hex, NK_TEXT_ALIGN_LEFT);
				}
			}
			nk_layout_row_end(g->ctx);

			nk_group_end(g->ctx);
		}
	}
	nk_end(g->ctx);

	return !(nk_window_is_hidden(g->ctx, NAME) || nk_window_is_closed(g->ctx, NAME));
}
