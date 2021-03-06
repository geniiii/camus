#include <chip8.h>

#include <stdlib.h>

#include <chip8/optable.h>

#define OP(op) chip8_op_##op

/* 5 bytes * 16 characters */
static const u8 FONTSET[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
	0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
	0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
	0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
	0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
	0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
	0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
	0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
	0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
	0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
	0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
	0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
	0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
	0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
	0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
	0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

void chip8_init(struct chip8* restrict c) {
	chip8_cpu_init(&c->cpu);
	camus_delta_init(&c->delta);

	memset(c->mem, 0, sizeof c->mem);
	chip8_reset(c);

	/* Copy fontset */
	memcpy(&c->mem[FONTSET_START], FONTSET, sizeof FONTSET);
}

void chip8_reset(struct chip8* restrict c) {
	chip8_cpu_reset(&c->cpu);
	c->cpu.pc = PROG_BEGIN;

	memset(c->stack, 0, sizeof c->stack);

	c->sound = 0;
	c->delay = 0;

	c->running = true;
}

u8 chip8_load(struct chip8* restrict c, const char* restrict fname) {
	FILE* fp = fopen(fname, "rb");
	if (!fp) {
		fputs("Failed to open file", stderr);
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	size_t fsize = ftell(fp);
	if (fsize > MEM_SIZE - PROG_BEGIN) {
		fputs("File too large", stderr);
		return 1;
	}
	rewind(fp);

	fread(&c->mem[PROG_BEGIN], fsize, sizeof(u8), fp);
	fclose(fp);

	return 0;
}

void chip8_emulate_cycle(struct chip8* restrict c) {
	static const chip8_op_ptr op_ltable[16] = {
		OP(0xxx), OP(1nnn), OP(2nnn), OP(3xnn),
		OP(4xnn), OP(5xy0), OP(6xnn), OP(7xnn),
		OP(8xxx), OP(9xy0), OP(annn), OP(bnnn),
		OP(cxnn), OP(dxyn), OP(exxx), OP(fxxx)};
	chip8_op_fetch(c);

	chip8_op_ptr op = op_ltable[c->cpu.op.op];
	op(c);
}
