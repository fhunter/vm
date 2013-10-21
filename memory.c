/** \brief VM Memory access and paging routines
  * \author Nikolay Zamotaev
  * \file memory.c
  */
#include <stdio.h>
#include "memory.h"
#include "vm.h"

/** \brief Free the memory and clean bitmaps
  * \param machine -- virtual machine state for the operation
  */
void reinit_ram( struct t_virtual_machine *machine )
{
  int i;
  for( i = 0; i < RAMSIZE / PAGESIZE; i++ ) {
    if( machine->ram_pointers[i] != NULL ) {
      free( machine->ram_pointers[i] );
      machine->ram_pointers[i] = NULL;
    }
  }
  bitmap_clearall( machine->ram_bitmap );
};

/** \brief Write the word to memory address
  * \param machine -- virtual machine state for the operation
  * \param addr -- write address
  * \param value -- data to write
  * \todo Unimplemented page swapping
  * \todo Untested
  */
void ivm_mem_put( struct t_virtual_machine *machine,
                         uint16_t addr, uint16_t value )
{
  if( addr < 0xfc00 ) {
    //Access RAM
    if( bitmap_get( machine->ram_bitmap, addr ) ) {
      //the memory was already accessed
      if( machine->ram_pointers[addr >> 8] == NULL ) {
        //memory is swapped out
        //need to get it from disk
        //FIXME: not implemented
      }
      else {
        //memory is available
        machine->ram_pointers[addr >> 8][addr & 0xffu] = value;
      }
    }
    else {
      machine->ram_pointers[addr >> 8] =
        calloc( PAGESIZE, sizeof( uint16_t ) );
      machine->ram_pointers[addr >> 8][addr & 0xfful] = value;
      //Update bitmap
      bitmap_set( machine->ram_bitmap, addr );
    }
  }
  else {
    //Video RAM
    if( addr > 0xff00 ) {
      printf( "\0337\033[%d;%dH%c\0338", ( 0xffff - addr ) / 16,
              ( 0xffff - addr ) % 16, value );
    }
    //Access peripherials
  }
};

/** \brief Read word from memory address
  * \param machine -- virtual machine state for the operation
  * \param addr -- read address
  * \return word at memory address
  * \todo Unimplemented page swapping
  * \todo Untested
  * \note Returns 0, if the page was never accessed
  */
uint16_t ivm_mem_get( struct t_virtual_machine *machine,
                             uint16_t addr )
{
  if( addr < 0xfc00 ) {
    //Access RAM
    if( bitmap_get( machine->ram_bitmap, addr ) ) {
      if( machine->ram_pointers[addr >> 8] == NULL ) {
        //Address not found, must fetch from disk.
        //Unimplemented
        return 0;
      }
      else {
        //Address found
        return machine->ram_pointers[addr >> 8][addr & 0xfful];
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
