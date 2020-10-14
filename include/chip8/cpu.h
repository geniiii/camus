#ifndef CPU_H
#define CPU_H

#include <util/types.h>
#include <chip8/opcode.h>

#define NUM_REGS  16
#define CARRY_REG 0xF

typedef struct chip8_cpu {
	u16			   i;
	u16			   pc;
	chip8_opcode_t op;
	u8			   sp;

	u8 v[NUM_REGS];

	u8 sound;
	u8 delay;

	bool halt;
} chip8_cpu_t;

#endif