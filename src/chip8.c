#include "chip8.h"

#define FONTSET_SIZE

/* 5 bytes * 16 characters */
static byte FONTSET[5 * 16] = {
	/* clang-format off */
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
	/* clang-format on */
};

void chip8_reset(chip8_t* c) {
	memset(c, 0, sizeof c);
	c->pc = PROG_BEGIN;

	/* Copy fontset */
	memcpy(c->mem, FONTSET, sizeof FONTSET);
}

void chip8_load(chip8_t* c, byte* rom, size_t rom_size) {
	memset(c->mem[PROG_BEGIN], rom, rom_size);
}