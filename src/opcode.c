#include "opcode.h"

#include <math.h>
#include <stdlib.h>

#include "xoroshiro32pp.h"

#define OP(op) void chip8_op_##op(chip8_t* c)

#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000

#define X	(c->op >> 8 & 0xF)
#define Y	(c->op >> 4 & 0xF)
#define Vx	(c->v[X])
#define Vy	(c->v[Y])
#define Vf	(c->v[CARRY_REG])
#define N	(c->op & 0xF)
#define NN	(c->op & 0xFF)
#define NNN (c->op & 0xFFF)
#define PC	(c->pc)
#define I	(c->i)

void chip8_op_fetch(chip8_t* c) {
	c->op = c->mem[PC] << 8 | c->mem[PC + 1];
}

OP(00e0) {
	memset(c->screen, 0, sizeof c->screen);
	PC += 2;
} /* CLS - Clear display */
OP(00ee) {
	PC = c->stack[--c->sp];
	PC += 2;
} /* RET - Return from subroutine */

OP(1nnn) { PC = NNN; } /* JP   nnn - Jump to nnn */
OP(2nnn) {
	c->stack[c->sp++] = c->pc;
	PC				  = NNN;
} /* CALL nnn - Call subroutine at nnn */

OP(3xnn) { PC += Vx == NN ? 4 : 2; } /* SE  Vx, nn - Skip next opcode if Vx == nn */
OP(4xnn) { PC += Vx != NN ? 4 : 2; } /* SNE Vx, nn - Skip next opcode if Vx != nn */
OP(5xy0) { PC += Vx == Vy ? 4 : 2; } /* SE  Vx, Vy - Skip next opcode if Vx == Vy */
OP(9xy0) { PC += Vx != Vy ? 4 : 2; } /* SNE Vx, Vy - Skip next opcode if Vx != Vy */

OP(6xnn) {
	Vx = NN;
	PC += 2;
} /* LD  Vx, nn - Vx =  nn */
OP(7xnn) {
	Vx += NN;
	PC += 2;
} /* ADD Vx, nn - Vx += nn */

OP(8xy0) {
	Vx = Vy;
	PC += 2;
} /* LD   Vx, Vy - Vx =   Vy */
OP(8xy1) {
	Vx |= Vy;
	PC += 2;
} /* OR   Vx, Vy - Vx |=  Vy */
OP(8xy2) {
	Vx &= Vy;
	PC += 2;
} /* AND  Vx, Vy - Vx &=  Vy */
OP(8xy3) {
	Vx ^= Vy;
	PC += 2;
} /* XOR  Vx, Vy - Vx ^=  Vy */
OP(8xy4) {
	Vf = Vy > 0xFF - Vx;
	Vx += Vy;

	PC += 2;
} /* ADD  Vx, Vy - Vx +=  Vy, set Vf to 1 on carry */
OP(8xy5) {
	Vf = Vx > Vy;
	Vx -= Vy;

	PC += 2;
} /* SUB  Vx, Vy - Vx -=  Vy, set Vf to 0 on borrow */
OP(8xy6) {
	Vf = Vx & 1;
	Vx >>= 1;

	PC += 2;
} /* SHR  Vx, Vy - Vx >>= Vy, set Vf to old least significant bit */
OP(8xy7) {
	Vf = Vy > Vx;
	Vx = Vy - Vx;

	PC += 2;
} /* SUBN Vx, Vy - Vx =-  Vy, set Vf to 0 on borrow */
OP(8xye) {
	Vf = Vx >> 7;
	Vx <<= 1;

	PC += 2;
} /* SHL  Vx, Vy - Vx <<= Vy, set Vf to old most significant bit */

OP(annn) {
	I = NNN;
	PC += 2;
} /* LD I, nnn - I = nnn */
OP(bnnn) { PC = NNN + c->v[0]; } /* JP nnn + V[0] - Jump to nnn + V[0] */

OP(cxnn) {
	const u16 rnd = xs32pp_next() % 8;
	Vx			  = rnd & NN;

	PC += 2;
} /* RND Vx, nn - Vx = random & nn */

OP(dxyn) {
	Vf = 0;

	for (u8 y = 0; y < N; ++y) {
		u8 pixel = c->mem[I + y];
		for (u8 x = 0; x < 8; ++x) {
			if ((pixel & (0x80 >> x))) {
				u32 pos = (Vx + x) % SCREEN_WIDTH +
						  ((Vy + y) % SCREEN_HEIGHT) * SCREEN_WIDTH;
				if (c->screen[pos] == WHITE) {
					c->screen[pos] = BLACK;
					Vf			   = 1;
				} else {
					c->screen[pos] = WHITE;
				}
				c->draw = true;
			}
		}
	}

	PC += 2;
} /* DRW Vx, Vy, N - Display n-byte sprite starting at memory location I at Vx[y], set V[F] to 1 on collision */

OP(ex9e) {
	// TODO: Input
	PC += 2;
} /* SKP  Vx - Skip next opcode if key Vx is pressed */
OP(exa1) {
	// TODO: Input
	PC += 2;
} /* SKNP Vx - Skip next opcode if key Vx is not pressed */

OP(fx07) {
	Vx = c->delay;
	PC += 2;
} /* LD Vx, DT - Vx = delay timer */
OP(fx0a) {
	// TODO: Input
	PC += 2;
} /* LD Vx, K - Wait for key press, Vx = K */
OP(fx15) {
	c->delay = Vx;
	PC += 2;
} /* LD DT, Vx - Delay timer = Vx */
OP(fx18) {
	c->sound = Vx;
	PC += 2;
} /* LD ST, Vx - Sound timer = Vx */
OP(fx1e) {
	I += Vx;
	PC += 2;
} /* ADD I, Vx - I += Vx */
OP(fx29) {
	I = FONTSET[X];
	PC += 2;
} /* LD F, Vx - I = fontset[x] */
OP(fx33) {
	c->mem[I]	  = Vx / 100;
	c->mem[I + 1] = (Vx / 10) % 10;
	c->mem[I + 2] = (Vx % 100) % 10;
	PC += 2;
} /* BCD, Vx - Vx = BCD(Vx) */
OP(fx55) {
	memcpy(&c->mem[I], &c->v, X + 1);
	PC += 2;
} /* LD [I], Vx - Save V[0] - Vx to memory at I */
OP(fx65) {
	memcpy(&c->v, &c->mem[I], X + 1);
	PC += 2;
} /* LD Vx, [I] - Load from memory at I to V[0] - Vx */
