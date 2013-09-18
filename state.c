/** \file state.c
  * \brief Save/load virtual machine states
  * \author Nikolay Zamotaev
  */

#include <stdio.h>
#include "state.h"
#include "bitmaps.h"

/** \todo implement saving bitmaps
  * \todo document
  */
int vm_save_state( FILE * state_file, struct t_virtual_machine vm )
{
  int i = 0;
  fseek( state_file, 0, SEEK_SET );
  fwrite( vm.ivm_ds, sizeof( uint16_t ), 16, state_file );
  fwrite( vm.ivm_rs, sizeof( uint16_t ), 16, state_file );
  fwrite( &vm.ivm_dp, sizeof( uint8_t ), 1, state_file );
  fwrite( &vm.ivm_rp, sizeof( uint8_t ), 1, state_file );
  fwrite( &vm.ivm_pc, sizeof( uint16_t ), 1, state_file );
  save_bitmap( &vm.ram_bitmap, state_file );
  for( i = 0; i < RAMSIZE / PAGESIZE; i++ ) {
    if( bitmap_get( vm.ram_bitmap, i * PAGESIZE ) ) {
      if( vm.ram_pointers[i] != NULL ) {
        fseek( state_file, 512 + i * PAGESIZE, SEEK_SET );
        fwrite( vm.ram_pointers[i], sizeof( uint16_t ), PAGESIZE,
                state_file );
      };
    };
  };
  return 0;
}

/** \todo implement loading bitmaps
  * \todo document
  */
int vm_load_state( FILE * state_file, struct t_virtual_machine *vm )
{
  int i = 0;
  fseek( state_file, 0, SEEK_SET );
  fread( vm->ivm_ds, sizeof( uint16_t ), 16, state_file );
  fread( vm->ivm_rs, sizeof( uint16_t ), 16, state_file );
  fread( &vm->ivm_dp, sizeof( uint8_t ), 1, state_file );
  fread( &vm->ivm_rp, sizeof( uint8_t ), 1, state_file );
  fread( &vm->ivm_pc, sizeof( uint16_t ), 1, state_file );
  load_bitmap( &vm->ram_bitmap, state_file );
  if( bitmap_get( vm->ram_bitmap, vm->ivm_pc ) ) {
    fseek( state_file, 512 + i * PAGESIZE, SEEK_SET );
    for( i = 0; i < RAMSIZE / PAGESIZE; i++ ) {
      vm->ram_pointers[i] = NULL;
    }
    //Read only the page, where PC is located, other will be read on an as needed basis
    vm->ram_pointers[i] = calloc( PAGESIZE, sizeof( uint16_t ) );
    fread( vm->ram_pointers[i], sizeof( uint16_t ), PAGESIZE,
           state_file );
  }
  else {
    return -1;
  }
  return 0;
}

/** \todo implement
  * \todo document
  */
int vm_load_page( FILE * state_file, struct t_virtual_machine *vm,
                  uint16_t address )
{
  return -1;
};

/** \todo implement
  * \todo document
  */
int vm_save_page( FILE * state_file, struct t_virtual_machine *vm,
                  uint16_t address )
{
  return -1;
};
