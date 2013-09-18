/** \file state.c
  * \brief Save/load virtual machine states
  * \author Nikolay Zamotaev
  */

#include "state.h"

int vm_save_state( FILE * state_file, struct t_virtual_machine vm )
{
  int i = 0;
  fseek( state_file, 0, SEEK_SET );
  fwrite( vm.ivm_ds, sizeof( uint16_t ), 16, state_file );
  fwrite( vm.ivm_rs, sizeof( uint16_t ), 16, state_file );
  fwrite( &vm.ivm_dp, sizeof( uint8_t ), 1, state_file );
  fwrite( &vm.ivm_rp, sizeof( uint8_t ), 1, state_file );
  fwrite( &vm.ivm_pc, sizeof( uint16_t ), 1, state_file );
  //save_bitmap(vm.ram_bitmap, state_file);
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

int vm_load_state( FILE * state_file, struct t_virtual_machine *vm )
{
  return 0;
}
