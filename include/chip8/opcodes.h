#ifndef OPCODES_H
#define OPCODES_H

#include <chip8/chip8.h>

typedef void (*chip8_op_ptr)(chip8_t*);

#define OP(op) extern void chip8_op_##op(chip8_t* c);

/*
 nnn or addr - A 12-bit value, the lowest 12 bits of the opcode
 n or nibble - A 4-bit value, the lowest 4 bits of the opcode
 x - A 4-bit value, the lower 4 bits of the high byte of the opcode
 y - A 4-bit value, the upper 4 bits of the low byte of the opcode
 nn - An 8-bit value, the lowest 8 bits of the opcode
*/

extern void chip8_op_fetch(chip8_t* c);

OP(00e0) /* CLS - Clear display */
OP(00ee) /* RET - Return from subroutine */

OP(1nnn) /* JP   nnn - Jump to nnn */
OP(2nnn) /* CALL nnn - Call subroutine at nnn */

OP(3xnn) /* SE  Vx, nn   - Skip next opcode if Vx == nn */
OP(4xnn) /* SNE Vx, nn   - Skip next opcode if Vx != nn */
OP(5xy0) /* SE  Vx, Vy - Skip next opcode if Vx == Vy */
OP(9xy0) /* SNE Vx, Vy - Skip next opcode if Vx != Vy */

OP(6xnn) /* LD  Vx, nn - Vx =  nn */
OP(7xnn) /* ADD Vx, nn - Vx += nn */

OP(8xy0) /* LD   Vx, Vy - Vx =   Vy */
OP(8xy1) /* OR   Vx, Vy - Vx |=  Vy */
OP(8xy2) /* AND  Vx, Vy - Vx &=  Vy */
OP(8xy3) /* XOR  Vx, Vy - Vx ^=  Vy */
OP(8xy4) /* ADD  Vx, Vy - Vx +=  Vy, set Vf to 1 on carry */
OP(8xy5) /* SUB  Vx, Vy - Vx -=  Vy, set Vf to 0 on borrow */
OP(8xy6) /* SHR  Vx, Vy - Vx >>= Vy, set Vf to old least significant bit */
OP(8xy7) /* SUBN Vx, Vy - Vx =-  Vy, set Vf to 0 on borrow */
OP(8xye) /* SHL  Vx, Vy - Vx <<= Vy, set Vf to old most significant bit */

OP(annn) /* LD I, nnn     - I = nnn */
OP(bnnn) /* JP nnn + V[0] - Jump to nnn + V[0] */

OP(cxnn) /* RND Vx, nn - Vx = random & nn */

OP(dxyn) /* DRW Vx, Vy, N - Display n-byte sprite starting at memory location I at Vx[y], set V[F] to 1 on collision */

OP(ex9e) /* SKP  Vx - Skip next opcode if key Vx is pressed */
OP(exa1) /* SKNP Vx - Skip next opcode if key Vx is not pressed */

OP(fx07) /* LD Vx,  DT - Vx = delay timer */
OP(fx0a) /* LD Vx,  K  - Wait for key press, Vx = K */
OP(fx15) /* LD DT,  Vx - Delay timer = Vx */
OP(fx18) /* LD ST,  Vx - Sound timer = Vx */
OP(fx1e) /* ADD I,  Vx - I += Vx */
OP(fx29) /* LD F,   Vx - I = fontset[x] */
OP(fx33) /* BCD,    Vx - Vx = BCD(Vx) */
OP(fx55) /* LD [I], Vx - Save V[0] - Vx to memory at I */
OP(fx65) /* LD Vx, [I] - Load from memory at I to V[0] - Vx */

#undef OP

#endif