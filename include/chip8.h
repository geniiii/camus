#ifndef CHIP8_H
#define CHIP8_H

#include "mem.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#define MEM_SIZE 0xFFF
#define PROG_BEGIN 0x200

#define SCREEN_SIZE 64 * 32

struct chip8_s {
	byte mem[MEM_SIZE];

	bool screen[SCREEN_SIZE];

	byte Vx;
	byte Vy;
	byte VF;
	int16_t I;

	int16_t pc;
	byte sp;

	int16_t stack[16];

	byte sound;
	byte delay;
};
typedef struct chip8_s chip8_t;

extern void chip8_reset(chip8_t* c);
extern void chip8_load(chip8_t* c, byte* rom, size_t rom_size);

#endif