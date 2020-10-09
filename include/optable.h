#ifndef OPTABLE_H
#define OPTABLE_H

#include "opcode.h"

#define OP(op) extern void chip8_op_##op(chip8_t* c);

OP(0xxx)
OP(1xxx)
OP(2xxx)
OP(3xxx)
OP(4xxx)
OP(5xxx)
OP(6xxx)
OP(7xxx)
OP(8xxx)
OP(9xxx)
OP(axxx)
OP(bxxx)
OP(cxxx)
OP(dxxx)
OP(exxx)
OP(fxxx)

#undef OP

#endif