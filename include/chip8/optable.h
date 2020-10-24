#ifndef OPTABLE_H
#define OPTABLE_H

#include <chip8/opcodes.h>

#define OP(op) extern void chip8_op_##op(struct chip8* restrict c);

OP(0xxx)
OP(8xxx)
OP(9xxx)
OP(exxx)
OP(fxxx)

#undef OP

#endif
