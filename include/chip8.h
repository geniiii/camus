#ifndef CHIP8_H
#define CHIP8_H

#include "mem.h"
#include "opcode.h"
#include "screen.h"

#define NUM_REGS  16
#define NUM_KEYS  16
#define CARRY_REG 0xF

#define MEM_SIZE	  0x1000
#define FONTSET_START 0x50
#define PROG_BEGIN	  0x200

#define STACK_SIZE 16

/* 5 bytes * 16 chars */
#define FONTSET_SIZE 5 * 16

extern const u8 FONTSET[FONTSET_SIZE];

typedef struct chip8 {
	u16			   i;
	u16			   pc;
	chip8_opcode_t op;
	u8			   sp;

	u8 v[NUM_REGS];

	u8	mem[MEM_SIZE];
	u16 stack[STACK_SIZE];

	u8 sound;
	u8 delay;

	chip8_screen_t screen;
} chip8_t;

extern void chip8_init(chip8_t* c);
extern void chip8_load(chip8_t* c, const char* filename);

extern void chip8_emulate_cycle(chip8_t* c);

#endif
