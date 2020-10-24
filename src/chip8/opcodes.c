#include <chip8/opcodes.h>

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_opengl.h>

#include <util/xoroshiro32pp.h>

#define KEY(key) SDL_SCANCODE_##key

const SDL_Scancode KEY_MAP[16] = {
	KEY(1), /* 0 */
	KEY(2), /* 1 */
	KEY(3), /* 2 */
	KEY(4), /* 3 */
	KEY(Q), /* 4 */
	KEY(W), /* 5 */
	KEY(E), /* 6 */
	KEY(R), /* 7 */
	KEY(A), /* 8 */
	KEY(S), /* 9 */
	KEY(D), /* A */
	KEY(F), /* B */
	KEY(Z), /* C */
	KEY(X), /* D */
	KEY(C), /* E */
	KEY(V)	/* F */
};

#define OP(op) void chip8_op_##op(struct chip8* restrict c)

#define X	(c->cpu.op.x)
#define Y	(c->cpu.op.y)
#define Vx	(c->cpu.v[X])
#define Vy	(c->cpu.v[Y])
#define Vf	(c->cpu.v[CARRY_REG])
#define N	(c->cpu.op.n)
#define NN	(c->cpu.op.nn)
#define NNN (c->cpu.op.nnn)
#define PC	(c->cpu.pc)
#define I	(c->cpu.i)

void chip8_op_fetch(struct chip8* restrict c) {
	c->cpu.op.opcode = c->mem[PC] << 8 | c->mem[PC + 1];
}

OP(00e0) {
	chip8_screen_clear(&c->screen);
	PC += 2;
} /* CLS - Clear display */
OP(00ee) {
	PC = c->stack[--c->cpu.sp];
	PC += 2;
} /* RET - Return from subroutine */

OP(1nnn) { PC = NNN; } /* JP   nnn - Jump to nnn */
OP(2nnn) {
	c->stack[c->cpu.sp++] = c->cpu.pc;
	PC					  = NNN;
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
	Vf = (bool) (Vx >= Vy);
	Vx -= Vy;

	PC += 2;
} /* SUB  Vx, Vy - Vx -=  Vy, set Vf to 0 on borrow */
OP(8xy6) {
	Vf = Vx & 1;
	Vx >>= 1;

	PC += 2;
} /* SHR  Vx, Vy - Vx >>= Vy, set Vf to old least significant bit */
OP(8xy7) {
	Vf = Vy >= Vx;
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
OP(bnnn) { PC = NNN + c->cpu.v[0]; } /* JP nnn + V[0] - Jump to nnn + V[0] */

OP(cxnn) {
	const u8 rnd = xs32pp_next() % 8;
	Vx			 = rnd & NN;

	PC += 2;
} /* RND Vx, nn - Vx = random & nn */

OP(dxyn) {
	Vf = 0;

	for (u8 y = 0; y < N; ++y) {
		u8 pixel = c->mem[I + y];
		for (u8 x = 0; x < 8; ++x) {
			if (pixel & (0x80 >> x)) {
				u32 pos = (Vx + x) % SCREEN_WIDTH +
						  ((Vy + y) % SCREEN_HEIGHT) * SCREEN_WIDTH;
				if (c->screen.pixels[pos] == ON_COLOR) {
					c->screen.pixels[pos] = OFF_COLOR;
					Vf					  = 1;
				} else {
					c->screen.pixels[pos] = ON_COLOR;
				}
				c->screen.draw = true;
			}
		}
	}

	PC += 2;
} /* DRW Vx, Vy, N - Display n-byte sprite starting at memory location I at Vx[y], set V[F] to 1 on collision */

OP(ex9e) { PC += SDL_GetKeyboardState(NULL)[KEY_MAP[Vx]] ? 4 : 2; }	 /* SKP  Vx - Skip next opcode if key Vx is pressed */
OP(exa1) { PC += !SDL_GetKeyboardState(NULL)[KEY_MAP[Vx]] ? 4 : 2; } /* SKNP Vx - Skip next opcode if key Vx is not pressed */

OP(fx07) {
	Vx = c->delay;
	PC += 2;
} /* LD Vx, DT - Vx = delay timer */
OP(fx0a) {
	/* We don't increase the program counter until a key is pressed, meaning we loop the same opcode */
	for (u8 i = 0; i < NUM_KEYS; ++i) {
		if (SDL_GetKeyboardState(NULL)[KEY_MAP[i]]) {
			Vx = i;
			PC += 2;
			return;
		}
	}
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
	I = Vx * 5;
	PC += 2;
} /* LD F, Vx - I = fontset[x] */
OP(fx33) {
	c->mem[I]	  = Vx / 100;
	c->mem[I + 1] = (Vx / 10) % 10;
	c->mem[I + 2] = (Vx % 100) % 10;
	PC += 2;
} /* BCD, Vx - Vx = BCD(Vx) */
OP(fx55) {
	memcpy(&c->mem[I], &c->cpu.v, X + 1);
	PC += 2;
} /* LD [I], Vx - Save V[0] - Vx to memory at I */
OP(fx65) {
	memcpy(&c->cpu.v, &c->mem[I], X + 1);
	PC += 2;
} /* LD Vx, [I] - Load from memory at I to V[0] - Vx */
