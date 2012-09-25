#include <stdlib.h>
#include "dcpu16.h"

uint16_t *get_operand( context_t * context, uint16_t a,
                       uint16_t * temp )
{
  uint16_t *operand_a = NULL;
  if( ( a >= 0x0 ) && ( a <= 0x7 ) ) {
    //register
    operand_a = &( context->registers[a] );
  }
  else if( a <= 0x0f ) {
    //[register]
    operand_a = &( context->ram[context->registers[a - 0x08]] );
  }
  else if( a <= 0x17 ) {
    //[next_word+register]
    operand_a =
      &( context->ram[context->registers[a - 0x10] +
                      context->registers[REGISTER_PC]++] );
  }
  else if( a == 0x18 ) {
    operand_a = &context->ram[--context->registers[REGISTER_SP]];
    //pop
  }
  else if( a == 0x19 ) {
    operand_a = &context->ram[context->registers[REGISTER_SP]];
    //peek
  }
  else if( a == 0x1a ) {
    operand_a = &context->ram[context->registers[REGISTER_SP]++];
  }
  else if( a == 0x1b ) {
    operand_a = &context->registers[REGISTER_SP];
    //sp
  }
  else if( a == 0x1c ) {
    operand_a = &context->registers[REGISTER_PC];
    //pc
  }
  else if( a == 0x1d ) {
    operand_a = &context->registers[REGISTER_O];
    //o
  }
  else if( a == 0x1e ) {
    //[next_word]
    *temp = context->ram[context->registers[REGISTER_PC]++];
    operand_a = &context->ram[*temp];
  }
  else if( a == 0x1f ) {
    //next_word
    *temp = context->ram[context->registers[REGISTER_PC]++];
    operand_a = temp;
  }
  else {
    //literal, a-0x20
    *temp = a - 0x20;
    operand_a = temp;
  };
  return operand_a;
};

void process_one_instruction( context_t * context )
{
  uint16_t instruction;
  instruction = context->ram[context->registers[REGISTER_PC]++];
  uint16_t b, a, o;
  uint16_t *operand_a, *operand_b;
  uint16_t toper_a, toper_b;
  o = instruction & 0x0ful;
  a = ( instruction >> 4 ) & 0x3f;
  b = ( instruction >> 10 ) & 0x3f;
  switch ( o ) {
    case 0x0:
//non-basic
      operand_a = get_operand( context, b, &toper_b );
      switch ( a ) {
        case 0x00:
          //reserved;
          break;
        case 0x01:
          //JSR a: stack <-pc++,pc=a;
          break;
        default:
          //reserved
          break;
      };
      break;
    case 0x1:
//set a,b (a<-b)
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_b;
      break;
    case 0x2:
//add
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a + *operand_b;
//      context->registers[REGISTER_O]= //FIXME: fill with overflow
      break;
    case 0x3:
//sub
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a - *operand_b;
//      context->registers[REGISTER_O]= //FIXME: fill with underflow
      break;
    case 0x4:
//mul
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a * *operand_b;
//      context->registers[REGISTER_O]= //FIXME: fill with underflow
      break;
    case 0x5:
//div
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a / *operand_b;
//      context->registers[REGISTER_O]= //FIXME: fill with underflow
      break;
    case 0x6:
//mod
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a % *operand_b;
//      context->registers[REGISTER_O]= //FIXME: fill with underflow
      break;
    case 0x7:
//shl
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a << *operand_b;
//      context->registers[REGISTER_O]= //FIXME: fill with underflow
      break;
    case 0x8:
//shr
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a >> *operand_b;
//      context->registers[REGISTER_O]= //FIXME: fill with underflow
      break;
    case 0x9:
//and
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a & *operand_b;
      break;
    case 0xa:
//bor
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a | *operand_b;
      break;
    case 0xb:
//xor
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      *operand_a = *operand_a ^ *operand_b;
      break;
    case 0xc:
//ife
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( *operand_a == *operand_b ) {
        //perform next command
      }
      else {
        //skip
      };
      break;
    case 0xd:
//ifn
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( *operand_a != *operand_b ) {
        //perform next command
      }
      else {
        //skip
      };
      break;
    case 0xe:
//ifg
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( *operand_a > *operand_b ) {
        //perform next command
      }
      else {
        //skip
      };
      break;
    case 0xf:
//ifb
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( ( *operand_a & *operand_b ) != 0 ) {
        //perform next command
      }
      else {
        //skip
      };
      break;
  };
};
