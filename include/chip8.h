#ifndef CHIP8_H
#define CHIP8_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "mem.h"

#define NUM_REGS  16
#define CARRY_REG 0xF

#define MEM_SIZE	  0x1000
#define FONTSET_START 0x50
#define PROG_BEGIN	  0x200

#define STACK_SIZE 16

/* 5 bytes * 16 chars */
#define FONTSET_SIZE  5 * 16
#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32
#define SCREEN_SIZE	  SCREEN_WIDTH* SCREEN_HEIGHT

extern const u8 FONTSET[FONTSET_SIZE];

typedef struct chip8 {
	u16 i;
	u16 pc;
	u16 op;
	u8	sp;

	u8 v[NUM_REGS];

	u8	mem[MEM_SIZE];
	u16 stack[STACK_SIZE];

	u8 sound;
	u8 delay;

	u32	 screen[SCREEN_SIZE];
	bool draw;
} chip8_t;

extern void chip8_init(chip8_t* c);
extern void chip8_load(chip8_t* c, const char* filename);

extern void chip8_emulate_cycle(chip8_t* c);

#endif
