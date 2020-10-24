#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

#include <util/types.h>
#include <chip8/opcode.h>

#define NUM_REGS  16
#define CARRY_REG 0xF

struct chip8_cpu {
	u16				   i;
	u16				   pc;
	union chip8_opcode op;
	u8				   sp;

	u8 v[NUM_REGS];

	bool halt;
};

#endif
