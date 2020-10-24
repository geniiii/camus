#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

#include <util/types.h>
#include <chip8/opcode.h>

#define NUM_REGS  16
#define CARRY_REG 0xF

struct chip8_cpu {
	struct {
		double timer_speed;
		u8	   ips_mult;
	} speed;

	u16				   i;
	u16				   pc;
	union chip8_opcode op;
	u8				   sp;

	u8 v[NUM_REGS];

	bool halt;
};

extern void chip8_cpu_init(struct chip8_cpu* restrict cpu);
extern void chip8_cpu_reset(struct chip8_cpu* restrict cpu);

#endif
