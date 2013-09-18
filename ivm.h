#ifndef __IVM_H__
#define __IVM_H__
/** \file ivm.h
  * \brief J1 Virtual machine code and definitions
  */
#include "bitmaps.h"
#include "memory.h"

#include <inttypes.h>

/** \brief extract opcode */
#define OP(x)  ((x) & (0xe000))
/** \brief extract call/jump/jumpz argument */
#define ARG(x) ((x) & (0x1fff))
/** \brief extract literal */
#define ARG_LIT(x) ((x) & 0x7fff)

/** \brief extract alu opcode */
#define ALU_OP(x)   (x & (0xf << 8))
/** \brief extract data stack pointer movement */
#define ALU_DS(x)		(x & (0x3 << 0))
/** \brief extract return stack pointer movement */
#define ALU_RS(x)		(x & (0x3 << 2))

/* Operation types */
enum
{
  OP_JMP = 0x0000,
  OP_JZ = 0x2000,
  OP_CALL = 0x4000,
  OP_ALU = 0x6000,
  OP_LIT = 0x8000
};

/* ALU operation types */
enum
{
  ALU_OP_T = 0,
  ALU_OP_N = 1,
  ALU_OP_PLUS = 2,
  ALU_OP_AND = 3,
  ALU_OP_OR = 4,
  ALU_OP_XOR = 5,
  ALU_OP_NEG = 6,
  ALU_OP_EQ = 7,
  ALU_OP_LESS = 8,
  ALU_OP_RSHIFT = 9,
  ALU_OP_DEC = 10,
  ALU_OP_R = 11,
  ALU_OP_MEM = 12,
  ALU_OP_LSHIFT = 13,
  ALU_OP_DEPTH = 14,
  ALU_OP_ULESS = 15
};

void ivm_reset( struct t_virtual_machine *machine );
void ivm_step( struct t_virtual_machine *machine, uint16_t word );
#endif /* __IVM_H__ */
