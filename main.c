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
#include "memory.h"

/** \brief main program function
  */
int main( int argc, char **argv )
{
  int i = 0, total = 0;
  uint16_t counter = 0;
  uint16_t buf[PAGESIZE];
  struct t_virtual_machine machine;
  machine.ram_bitmap=bitmap_create(RAMSIZE, PAGESIZE);
  bitmap_clearall(machine.ram_bitmap);
  while( ( i = read( STDIN_FILENO, buf, sizeof( buf ) ) ) > 0 ) {
    //allocate ram for program
    machine.ram_pointers[counter] =
      calloc( PAGESIZE, sizeof( uint16_t ) );
    //copy loaded page
    memcpy( machine.ram_pointers[counter], buf, i );
    //set page occupied bit
    bitmap_set( machine.ram_bitmap, counter * PAGESIZE + 1 );
    counter++;
    total += i;
  };
  printf( "loaded %d bytes\n", total );
  ivm_reset( &machine );
  i = 0;
  while( 1 ) {
    ivm_step( &machine, ivm_mem_get( &machine, machine.ivm_pc ) );
    printf
      ( "\033[18;1H i=%04x ivm_pc=%04x  ivm_dp=%08x  ivm_rs=%08x\n",
        i, machine.ivm_pc, machine.ivm_dp, machine.ivm_rs );
    usleep( 100000 );
    i++;
  };
  bitmap_destroy(machine.ram_bitmap);
  return 0;
};
