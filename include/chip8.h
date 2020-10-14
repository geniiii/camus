#ifndef CHIP8_H
#define CHIP8_H

#include <chip8/opcode.h>
#include <chip8/screen.h>
#include <chip8/cpu.h>

#define NUM_KEYS 16

#define MEM_SIZE   0x1000
#define STACK_SIZE 16

#define FONTSET_START 0x0
#define PROG_BEGIN	  0x200

/* 5 bytes * 16 chars */
#define FONTSET_SIZE 5 * 16

extern const u8 FONTSET[FONTSET_SIZE];

typedef struct chip8 {
	chip8_cpu_t cpu;

	u8	mem[MEM_SIZE];
	u16 stack[STACK_SIZE];

	chip8_screen_t screen;
	u8			   sound;

	u8 delay;

	bool running;
} chip8_t;

extern void chip8_init(chip8_t* c);
extern u8	chip8_load(chip8_t* c, const char* filename);

extern void chip8_emulate_cycle(chip8_t* c);

#endif
