#ifndef __IVM_H__
#define __IVM_H__
/** \file ivm.h
  * \brief J1 Virtual machine code and definitions
  */

#include <inttypes.h>

#define OP(x)  ((x) & (0xe000))
#define ARG(x) ((x) & (0x1fff))
#define ARG_LIT(x) ((x) & 0x7fff)

#define ALU_OP(x)   (x & (0xf << 8))
#define ALU_DS(x)		(x & (0x3 << 0))
#define ALU_RS(x)		(x & (0x3 << 2))

static void ivm_mem_put( uint16_t addr, uint16_t value );
static uint16_t ivm_mem_get( uint16_t addr );

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

/** \brief Data stack */
static uint16_t ivm_ds[16];
/** \brief Return stack */
static uint16_t ivm_rs[16];
/** \brief top of data stack */
static int8_t ivm_dp;
/** \brief top of return stack */
static int8_t ivm_rp;
/** \brief Program counter for virtual machine */
static int16_t ivm_pc;

/** \brief Reset the state of virtual machine to initial one */
static inline void ivm_reset(  )
{
  ivm_pc = 0;
  ivm_dp = ivm_rp = -1;
}

/** \brief Perform one command step for VM
  * \param word -- instruction word to run
  */
static inline void ivm_step( uint16_t word )
{
  if( word & 0x8000 ) {
    ivm_ds[++ivm_dp] = ARG_LIT( word );
    ivm_pc++;
    return;
  }

  switch ( OP( word ) ) {
    case OP_JZ:
      if( ivm_ds[ivm_dp--] == 0 ) {
        ivm_pc = ARG( word );
      }
      else {
        ivm_pc++;
      }
      break;
    case OP_JMP:
      ivm_pc = ARG( word );
      break;
    case OP_CALL:
      ivm_rs[++ivm_rp] = ivm_pc + 1;
      ivm_pc = ARG( word );
      break;
    case OP_ALU:{
        uint16_t t, n, r, res;
        t = ivm_ds[ivm_dp];
        if( ivm_dp > 0 ) {
          n = ivm_ds[ivm_dp - 1];
        }
        r = ivm_rs[ivm_rp];

        ivm_pc++;

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
            res = ivm_mem_get( t );
            break;
          case ALU_OP_LSHIFT:
            res = ( n << t );
            break;
          case ALU_OP_DEPTH:
            res = ivm_dp + 1;
            break;
          case ALU_OP_ULESS:
            res = ( n < t );
            break;
        }

        switch ( ALU_DS( word ) ) {
          case ( 1 << 0 ):
            ivm_dp++;
            break;
          case ( 2 << 0 ):
            ivm_dp--;
            break;
        }

        switch ( ALU_RS( word ) ) {
          case ( 1 << 2 ):
            ivm_rp++;
            break;
          case ( 2 << 2 ):
            ivm_rp--;
            break;
        }

        if( word & ( 1 << 5 ) ) {
          ivm_mem_put( t, n );
        }
        if( word & ( 1 << 6 ) ) {
          ivm_rs[ivm_rp] = t;
        }
        if( word & ( 1 << 7 ) ) {
          ivm_ds[ivm_dp - 1] = t;
        }
        if( word & ( 1 << 12 ) ) {
          /* TODO: set 0x7fff return address on reset? */
          if( ivm_rp < 0 ) {    /* exit condition */
            ivm_pc = 0x7fff;
          }
          else {
            ivm_pc = r;
          }
        }

        if( ivm_dp >= 0 ) {
          ivm_ds[ivm_dp] = res;
        }
        break;
      }
  }
}

#endif /* __IVM_H__ */
