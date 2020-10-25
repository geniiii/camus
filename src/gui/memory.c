#include <gui/memory.h>
#include <util/hex.h>

#define NAME "Memory"

bool camus_gui_memory(struct camus_gui* restrict g) {
	if (nk_begin(g->ctx, NAME, nk_rect(50, 50, 350, 400), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_NO_SCROLLBAR)) {
		const struct nk_rect total_space   = nk_window_get_content_region(g->ctx);
		const float			 panel_padding = g->ctx->style.window.padding.x;

		nk_layout_row_begin(g->ctx, NK_STATIC, 10, 1 + 0x8);
		{
			nk_layout_row_push(g->ctx, 35 + panel_padding);
			nk_spacing(g->ctx, 1);

			for (u8 i = 0; i <= 0xF; i += 2) {
				char hex[2 + 1];
				u16_to_hex(i, hex, 2);

				nk_layout_row_push(g->ctx, 30);
				nk_label(g->ctx, hex, NK_TEXT_ALIGN_LEFT);
			}
		}
		nk_layout_row_end(g->ctx);

		nk_layout_row_static(g->ctx, total_space.h - panel_padding * 2, total_space.w, 1);
		if (nk_group_begin(g->ctx, "Contents", 0)) {
			nk_layout_row_begin(g->ctx, NK_STATIC, 10, 1 + 0x8);
			{
				for (u16 i = 1; i < MEM_SIZE + 1; i += 2) {
					char hex[4 + 1];

					if ((i - 1) % 0x10 == 0) {
						nk_layout_row_push(g->ctx, 35);
						u16_to_hex(i - 1, hex, 4);

						nk_label(g->ctx, hex, NK_TEXT_ALIGN_LEFT);
					}

					u16_to_hex(g->c->mem[i - 1] << 8 | g->c->mem[i], hex, 4);
					nk_layout_row_push(g->ctx, 30);
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
