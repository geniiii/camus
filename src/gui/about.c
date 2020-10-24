#include <gui/about.h>
#include <util/config.h>

#define NAME "About"

#define NAME_VERSION_SHA1 NAME_VER "-" GIT_SHA1
#define LICENSE			  PROJECT_NAME " is licensed under the MIT license. See the accompanying LICENSE file for more information."

bool camus_gui_about(struct camus_gui* restrict g) {
	if (nk_begin(g->ctx, NAME, nk_rect(50, 50, 300, 150), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_NO_SCROLLBAR)) {
		nk_layout_row_dynamic(g->ctx, 30, 1);
		nk_label(g->ctx, NAME_VERSION_SHA1, NK_TEXT_CENTERED);

		nk_layout_row_dynamic(g->ctx, 100, 1);
		nk_label_wrap(g->ctx, LICENSE);
	}
	nk_end(g->ctx);

	return !(nk_window_is_hidden(g->ctx, NAME) || nk_window_is_closed(g->ctx, NAME));
}
