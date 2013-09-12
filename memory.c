/** \brief VM Memory access and paging routines
  * \author Nikolay Zamotaev
  * \file memory.c
  */
#include "memory.h"

/*
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
*/
