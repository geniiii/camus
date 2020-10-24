#ifndef OPCODE_H
#define OPCODE_H

#include <util/types.h>

typedef union chip8_opcode {
	u16 opcode;

	struct {
		u8 n : 4;
		u8 y : 4;
		u8 x : 4;
		u8 op : 4;
	};

	struct {
		u8 nn : 8;
	};

	struct {
		u16 nnn : 12;
	};
} chip8_opcode_t;

#endif
