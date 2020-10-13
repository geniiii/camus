#include <chip8/optable.h>

#define X	(c->cpu.op.n)
#define XX	(c->cpu.op.nn)
#define XXX (c->cpu.op.nnn)

#define OP(op)	void chip8_op_##op(chip8_t* c)
#define COP(op) chip8_op_##op(c)

OP(0xxx) {
	switch (XX) {
		case 0xE0: COP(00e0); break;
		case 0xEE: COP(00ee); break;
	}
}
OP(8xxx) {
	switch (X) {
		case 0x0: COP(8xy0); break;
		case 0x1: COP(8xy1); break;
		case 0x2: COP(8xy2); break;
		case 0x3: COP(8xy3); break;
		case 0x4: COP(8xy4); break;
		case 0x5: COP(8xy5); break;
		case 0x6: COP(8xy6); break;
		case 0x7: COP(8xy7); break;
		case 0xE: COP(8xye); break;
	}
}
OP(exxx) {
	switch (XX) {
		case 0x9E: COP(ex9e); break;
		case 0xA1: COP(exa1); break;
	}
}
OP(fxxx) {
	switch (XX) {
		case 0x07: COP(fx07); break;
		case 0x0A: COP(fx0a); break;
		case 0x15: COP(fx15); break;
		case 0x18: COP(fx18); break;
		case 0x1E: COP(fx1e); break;
		case 0x29: COP(fx29); break;
		case 0x33: COP(fx33); break;
		case 0x55: COP(fx55); break;
		case 0x65: COP(fx65); break;
	}
}
