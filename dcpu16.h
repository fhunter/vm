#ifndef DCPU16_H_
#define DCPU16_H_
#include <stdint.h>

enum
{
  REGISTER_A = 0,
  REGISTER_B,
  REGISTER_C,
  REGISTER_X,
  REGISTER_Y,
  REGISTER_Z,
  REGISTER_I,
  REGISTER_J,
  REGISTER_SP,
  REGISTER_PC,
  REGISTER_O
};

typedef struct
{
  uint16_t ram[( 1 << 16 )];
  uint16_t registers[11];
} context_t;

void process_one_instruction( context_t * context );
void print_status( context_t * context );

#endif
