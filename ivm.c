/** \file ivm.c
  * \brief J1 Virtual machine code and definitions
  */
#include "bitmaps.h"
#include "memory.h"
#include "ivm.h"

#include <inttypes.h>

/** \brief Reset the state of virtual machine to initial one
  * \param machine -- virtual machine state to reset
  */
void ivm_reset( struct t_virtual_machine *machine )
{
  machine->ivm_pc = 0;
  machine->ivm_dp = -1;
  machine->ivm_rp = 0;
}

/** \brief Perform one command step for VM
  * \param machine -- virtual machine descriptor from which to run
  * \param word -- instruction word to run
  */
void ivm_step( struct t_virtual_machine *machine, uint16_t word )
{
  if( word & 0x8000 ) {
    machine->ivm_ds[++machine->ivm_dp] = ARG_LIT( word );
    machine->ivm_pc++;
    return;
  }

  switch ( OP( word ) ) {
    case OP_JZ:
      if( machine->ivm_ds[machine->ivm_dp--] == 0 ) {
        machine->ivm_pc = ARG( word );
      }
      else {
        machine->ivm_pc++;
      }
      break;
    case OP_JMP:
      machine->ivm_pc = ARG( word );
      break;
    case OP_CALL:
      machine->ivm_rs[++machine->ivm_rp] = machine->ivm_pc + 1;
      machine->ivm_pc = ARG( word );
      break;
    case OP_ALU:{
        uint16_t t, n = 0, r, res;
        t = machine->ivm_ds[machine->ivm_dp];
        if( machine->ivm_dp > 0 ) {
          n = machine->ivm_ds[machine->ivm_dp - 1];
        }
        r = machine->ivm_rs[machine->ivm_rp];

        machine->ivm_pc++;

        switch ( ALU_OP( word ) >> 8 ) {
          case ALU_OP_T:
            res = t;
            break;
          case ALU_OP_N:
            res = n;
            break;
          case ALU_OP_PLUS:
            res = t + n;
            break;
          case ALU_OP_AND:
            res = t & n;
            break;
          case ALU_OP_OR:
            res = t | n;
            break;
          case ALU_OP_XOR:
            res = t ^ n;
            break;
          case ALU_OP_NEG:
            res = ~t;
            break;
          case ALU_OP_EQ:
            res = ( t == n );
            break;
          case ALU_OP_LESS:
            res = ( n < t );
            break;
          case ALU_OP_RSHIFT:
            res = ( n >> t );
            break;
          case ALU_OP_DEC:
            res = t - 1;
            break;
          case ALU_OP_R:
            res = r;
            break;
          case ALU_OP_MEM:
            res = ivm_mem_get( machine, t );
            break;
          case ALU_OP_LSHIFT:
            res = ( n << t );
            break;
          case ALU_OP_DEPTH:
            res = machine->ivm_dp + 1;
            break;
          case ALU_OP_ULESS:
            res = ( n < t );
            break;
        }

        switch ( ALU_DS( word ) ) {
          case ( 1 << 0 ):
            machine->ivm_dp++;
            break;
          case ( 2 << 0 ):
            machine->ivm_dp--;
            break;
        }

        switch ( ALU_RS( word ) ) {
          case ( 1 << 2 ):
            machine->ivm_rp++;
            break;
          case ( 2 << 2 ):
            machine->ivm_rp--;
            break;
        }

        if( word & ( 1 << 5 ) ) {
          ivm_mem_put( machine, t, n );
        }
        if( word & ( 1 << 6 ) ) {
          machine->ivm_rs[machine->ivm_rp] = t;
        }
        if( word & ( 1 << 7 ) ) {
          machine->ivm_ds[machine->ivm_dp - 1] = t;
        }
        if( word & ( 1 << 12 ) ) {
          /* TODO: set 0x7fff return address on reset? */
          if( machine->ivm_rp < 0 ) {   /* exit condition */
            machine->ivm_pc = 0x7fff;
          }
          else {
            machine->ivm_pc = r;
          }
        }

        if( machine->ivm_dp >= 0 ) {
          machine->ivm_ds[machine->ivm_dp] = res;
        }
        break;
      }
  }
}
