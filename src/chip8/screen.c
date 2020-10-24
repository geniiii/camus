#include <GL/glew.h>

#include <chip8.h>
#include <chip8/screen.h>
#include <util/config.h>

u8 chip8_screen_init(struct chip8_screen* restrict s) {
	chip8_screen_clear(s);

	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fputs("Failed to init SDL (video)", stderr);
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	s->window = SDL_CreateWindow(NAME_VER, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 12, SCREEN_HEIGHT * 12, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	if (!s->window) {
		fputs("Failed to create window", stderr);
		return 1;
	}

	s->gl_ctx = SDL_GL_CreateContext(s->window);
	if (!s->gl_ctx) {
		fputs("Failed to create OpenGL context", stderr);
		return 1;
	}
	glewInit();

	glGenTextures(1, &s->texture);

	u32 win_width, win_height;
	SDL_GetWindowSize(s->window, &win_width, &win_height);
	glViewport(0, 0, win_width, win_height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, s->texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		NULL);

	glGenFramebuffers(1, &s->fbo);

	return 0;
}

u8 chip8_screen_draw(struct chip8_screen* restrict s) {
	glBindTexture(GL_TEXTURE_2D, s->texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, s->fbo);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
						   GL_TEXTURE_2D, s->texture, 0);

	u32 win_width, win_height;
	SDL_GetWindowSize(s->window, &win_width, &win_height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, win_height, win_width, 0,
					  GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

void chip8_screen_clear(struct chip8_screen* restrict s) {
	for (u16 i = 0; i < SCREEN_SIZE; ++i) {
		s->pixels[i] = OFF_COLOR;
	}

	s->draw = true;
}

void chip8_screen_destroy(struct chip8_screen* restrict s) {
	glDeleteFramebuffers(1, &s->fbo);
	s->fbo = 0;
	glDeleteTextures(1, &s->texture);
	s->texture = 0;
	SDL_GL_DeleteContext(s->gl_ctx);
	s->gl_ctx = NULL;

	SDL_DestroyWindow(s->window);
	s->window = NULL;
}
