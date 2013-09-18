/** \file state.h
  * \brief Save/load virtual machine states
  * \author Nikolay Zamotaev
  */

#ifndef _STATE__H_
#define _STATE__H_

#include <stdio.h>
#include "vm.h"

int vm_save_state( FILE * state_file, struct t_virtual_machine vm );
int vm_load_state( FILE * state_file, struct t_virtual_machine *vm );

#endif
