#include <stdlib.h>
#include <stdio.h>
#include "dcpu16.h"

const int register_count = 11;

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
  else if( ( a >= 0x1b ) && ( a <= 0x1d ) ) {
    operand_a = &context->registers[( a - 0x1b ) + REGISTER_SP];
    //sp,pc,o
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
  uint32_t overflow;
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
          context->ram[--context->registers[REGISTER_SP]] =
            context->registers[REGISTER_PC];
          context->registers[REGISTER_PC] = *operand_a;
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
      overflow = *operand_a + *operand_b;
      *operand_a = ( uint16_t ) overflow;
      context->registers[REGISTER_O] = overflow >> 16;
      break;
    case 0x3:
//sub
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( *operand_a < *operand_b ) {
        context->registers[REGISTER_O] = 0xffff;
      }
      else {
        context->registers[REGISTER_O] = 0;
      };
      *operand_a = *operand_a - *operand_b;
      break;
    case 0x4:
//mul
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      overflow = *operand_a * *operand_b;
      *operand_a = overflow;
      context->registers[REGISTER_O] = ( overflow >> 16 );
      break;
    case 0x5:
//div
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( *operand_b == 0 ) {
        *operand_a = 0;
        context->registers[REGISTER_O] = 0;
        break;
      }
      else {
        overflow = *operand_a << 16;
        overflow /= *operand_b;
        *operand_a = *operand_a / *operand_b;
        context->registers[REGISTER_O] = overflow;
      };
      break;
    case 0x6:
//mod
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( *operand_b == 0 ) {
        *operand_a = 0;
      }
      else {
        *operand_a = *operand_a % *operand_b;
      };
      break;
    case 0x7:
//shl
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      overflow = ( *operand_a << *operand_b ) >> 16;
      *operand_a = *operand_a << *operand_b;
      context->registers[REGISTER_O] = overflow;
      break;
    case 0x8:
//shr
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      overflow = ( *operand_a << 16 ) >> *operand_b;
      *operand_a = *operand_a >> *operand_b;
      context->registers[REGISTER_O] = overflow;
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
      if( *operand_a != *operand_b ) {
        context->registers[REGISTER_PC]++;
      };
      break;
    case 0xd:
//ifn
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( *operand_a == *operand_b ) {
        context->registers[REGISTER_PC]++;
      };
      break;
    case 0xe:
//ifg
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( *operand_a <= *operand_b ) {
        context->registers[REGISTER_PC]++;
      };
      break;
    case 0xf:
//ifb
      operand_a = get_operand( context, a, &toper_a );
      operand_b = get_operand( context, b, &toper_b );
      if( ( *operand_a & *operand_b ) == 0 ) {
        context->registers[REGISTER_PC]++;
      };
      break;
  };
};

void print_status( context_t * context )
{
  uint16_t i;
  static int step = 0;
  printf( "%04d :", step++ );
  for( i = REGISTER_A; i <= REGISTER_O; i++ ) {
    printf( " %3s:%04x ", ( char *[] ) {
            "A", "B", "C", "X", "Y", "Z", "I", "J", "SP", "PC",
            "O"}[i], context->registers[i] );
  };
  printf( "\n" );
};
