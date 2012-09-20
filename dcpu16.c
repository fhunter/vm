#include "dcpu16.h"

void push( context_t * context, uint16_t value )
{
  context->ram[context->registers[REGISTER_SP]++] = value;
};

uint16_t pop( context_t * context )
{
  return context->ram[--context->registers[REGISTER_SP]];
};

uint16_t peek( context_t * context )
{
  return context->ram[context->registers[REGISTER_SP]];
};

void process_one_instruction( context_t * context )
{
  uint16_t instruction;
  instruction = context->ram[context->registers[REGISTER_PC]++];
  uint16_t b, a, o;
  o = instruction & 0x0ful;
  a = ( instruction >> 4 ) & 0x3f;
  b = ( instruction >> 10 ) & 0x3f;
  switch ( o ) {
    case 0x0:
//non-basic
      break;
    case 0x1:
//set a,b (a<-b)
      break;
    case 0x2:
//add
      break;
    case 0x3:
//sub
      break;
    case 0x4:
//mul
      break;
    case 0x5:
//div
      break;
    case 0x6:
//mod
      break;
    case 0x7:
//shl
      break;
    case 0x8:
//shr
      break;
    case 0x9:
//and
      break;
    case 0xa:
//bor
      break;
    case 0xb:
//xor
      break;
    case 0xc:
//ife
      break;
    case 0xd:
//ifn
      break;
    case 0xe:
//ifg
      break;
    case 0xf:
//ifb
      break;
  };
};
