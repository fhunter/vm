/** \brief VM Memory access and paging routines
  * \author Nikolay Zamotaev
  * \file memory.c
  */
#include <stdio.h>
#include "memory.h"
#include "peripherials/peripherials.h"
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

static void ram_write( struct t_virtual_machine *machine,
                       uint16_t addr, uint16_t value )
{
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
};

static uint16_t ram_read( struct t_virtual_machine *machine,
                          uint16_t addr )
{
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
  if( addr < P_PERIPH_BASE ) {
    //Access RAM
    ram_write( machine, addr, value );
  }
  else {
    //Peripherial region
    // buttons, syscall, etc
    if( addr < ( P_BUTTONS_BASE + P_BUTTONS_LENGTH ) ) {
    }
    else if( addr < ( P_SYSCALL_BASE + P_SYSCALL_LENGTH ) ) {
    }
    else if( addr < ( P_TIMER_BASE + P_TIMER_LENGTH ) ) {
    }
    else if( ( addr > P_VIDEO_BASE )
             && ( addr <= ( P_VIDEO_BASE + P_VIDEO_LENGTH ) ) ) {
      //Video RAM
      ram_write( machine, addr, value );
      //FIXME: add abstraction
      printf( "\0337\033[%d;%dH%c\0338", ( 0xffff - addr ) / 16,
              ( 0xffff - addr ) % 16, value );
    }
    else {
      //Peripherial undefined
      ram_write( machine, addr, value );
    }
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
    return ram_read( machine, addr );
  }
  else {
    //Peripherial region
    // buttons, syscall, etc
    if( addr < ( P_BUTTONS_BASE + P_BUTTONS_LENGTH ) ) {
      return 0;
    }
    else if( addr < ( P_SYSCALL_BASE + P_SYSCALL_LENGTH ) ) {
      return 0;
    }
    else if( addr < ( P_TIMER_BASE + P_TIMER_LENGTH ) ) {
      return 0;
    }
    else if( ( addr > P_VIDEO_BASE )
             && ( addr <= ( P_VIDEO_BASE + P_VIDEO_LENGTH ) ) ) {
      //Video RAM
      return ram_read( machine, addr );
    }
    else {
      //Peripherial undefined
      return ram_read( machine, addr );
    }
  }
  return 0;
};
