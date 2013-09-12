/** \file main.c
  * \author Nikolay Zamotaev
  * \brief Main program file for programmable watch
  */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "launcher.h"
#include "ivm.h"
#include "bitmaps.h"

uint16_t *ram_pointers[RAMSIZE / PAGESIZE];
uint8_t ram_bitmap[get_bitmap_size( RAMSIZE, PAGESIZE )];

static void reinit_ram( uint16_t * ram_pointer[] )
{
  int i;
  for( i = 0; i < RAMSIZE / PAGESIZE; i++ ) {
    if( ram_pointer[i] != NULL ) {
      free( ram_pointer[i] );
      ram_pointer[i] = NULL;
    }
  }
  clr_allbits( ram_bitmap );
};

static void ivm_mem_put( uint16_t addr, uint16_t value )
{
  if( addr < 0xfc00 ) {
    //Access RAM
    if( get_bit( ram_bitmap, addr ) ) {
      //the memory was already accessed
      if( ram_pointers[addr >> 8] == NULL ) {
        //memory is swapped out
        //need to get it from disk
        //FIXME: not implemented
      }
      else {
        //memory is available
        ram_pointers[addr >> 8][addr & 0xffu] = value;
      }
    }
    else {
      ram_pointers[addr >> 8] =
        calloc( PAGESIZE, sizeof( uint16_t ) );
      ram_pointers[addr >> 8][addr & 0xfful] = value;
      //Update bitmap
      set_bit( ram_bitmap, addr );
    }
  }
  else {
    if( addr > 0xff00 ) {
      printf( "\033[%d;%dH%c", ( 0xffff - addr ) / 16,
              ( 0xffff - addr ) % 16, value );
      //Video RAM
    }
    //Access peripherials
  }
};

static uint16_t ivm_mem_get( uint16_t addr )
{
  if( addr < 0xfc00 ) {
    //Access RAM
    if( get_bit( ram_bitmap, addr ) ) {
      if( ram_pointers[addr >> 8] == NULL ) {
        //Address not found, must fetch from disk.
        //Unimplemented
        return 0;
      }
      else {
        //Address found
        return ram_pointers[addr >> 8][addr & 0xfful];
      }
    }
    else {
      //Page was never accessed for write. Memory is empty
      return 0;
    }
  }
  else {
    //Access peripherials
    return 0;
  }
};

int main( int argc, char **argv )
{
  int i = 0, total = 0;
  uint16_t counter = 0;
  uint16_t buf[PAGESIZE];
  while( ( i = read( STDIN_FILENO, buf, sizeof( buf ) ) ) > 0 ) {
    //allocate ram for program
    ram_pointers[counter] = calloc( PAGESIZE, sizeof( uint16_t ) );
    //copy loaded page
    memcpy( ram_pointers[counter], buf, i );
    //set page occupied bit
    set_bit( ram_bitmap, counter * PAGESIZE + 1 );
    counter++;
    total += i;
  };
  printf( "loaded %d bytes\n", total );
  ivm_reset(  );
  i = 0;
  while( 1 ) {
    ivm_step( ivm_mem_get( ivm_pc ) );
    printf
      ( "\033[18;1H i=%04x ivm_pc=%04x  ivm_dp=%08x  ivm_rs=%08x\n",
        i, ivm_pc, ivm_dp, ivm_rs );
    usleep( 100000 );
    i++;
  };
  return 0;
};
