/** \brief VM Memory access and paging routines
  * \author Nikolay Zamotaev
  * \file memory.h
  */
#ifndef _MEMORY__H_
#define _MEMORY__H_

#include <stdint.h>
#include <stdlib.h>
#include "vm.h"

void reinit_ram( struct t_virtual_machine *machine );
void ivm_mem_put( struct t_virtual_machine *machine,
                         uint16_t addr, uint16_t value );
uint16_t ivm_mem_get( struct t_virtual_machine *machine,
                             uint16_t addr );

#endif
