#include "chip8.h"

/* 5 bytes * 16 characters */
#define FONTSET_SIZE 5 * 16

/* clang-format off */
static byte FONTSET[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
};
/* clang-format on */

void chip8_reset(chip8_t* c) {
    memset(c, 0, sizeof c);
    c->pc = PROG_BEGIN;

    /* TODO: load fontset */
}

void chip8_load(chip8_t* c, byte* rom, size_t rom_size) {
    memset(c->mem[PROG_BEGIN], rom, rom_size);
}