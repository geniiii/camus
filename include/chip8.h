#ifndef CHIP8_H
#define CHIP8_H

#include <chip8/opcode.h>
#include <chip8/screen.h>
#include <chip8/cpu.h>
#include <util/delta.h>

#define NUM_KEYS 16

#define MEM_SIZE   0x1000
#define STACK_SIZE 16

#define FONTSET_START 0x0
#define PROG_BEGIN	  0x200

/* 5 bytes * 16 chars */
#define FONTSET_SIZE 5 * 16

struct chip8 {
	struct camus_delta delta;
	struct chip8_cpu   cpu;

	u8	mem[MEM_SIZE];
	u16 stack[STACK_SIZE];

	struct chip8_screen screen;

	u8 sound;
	u8 delay;

	bool running;
};

extern void chip8_init(struct chip8* restrict c);
extern void chip8_reset(struct chip8* restrict c);

extern u8 chip8_load(struct chip8* restrict c, const char* restrict filename);

extern void chip8_emulate_cycle(struct chip8* restrict c);

#endif
