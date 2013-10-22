#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "state.h"
#include "ivm.h"

int main( int argc, char **argv )
{
  struct t_virtual_machine vm;
  unsigned int i = 0;
  unsigned int total = 0;
  uint16_t counter = 0;
  uint16_t buf[PAGESIZE];
  vm.ram_bitmap = bitmap_create( RAMSIZE, PAGESIZE );
  memset( vm.ram_pointers, 0, sizeof( vm.ram_pointers ) );
  bitmap_clearall( vm.ram_bitmap );
  while( ( i = read( STDIN_FILENO, buf, sizeof( buf ) ) ) > 0 ) {
    //allocate ram for program
    vm.ram_pointers[counter] = calloc( PAGESIZE, sizeof( uint16_t ) );
    //copy loaded page
    memcpy( vm.ram_pointers[counter], buf, i );
    //set page occupied bit
    bitmap_set( vm.ram_bitmap, counter * PAGESIZE + 1 );
    counter++;
    total += i;
  };
  ivm_reset( &vm );
  vm_save_state( stdout, vm );
  return 0;
};
