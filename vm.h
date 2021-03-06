/** \file vm.h
  * \author Nikolay Zamotaev
  * \brief Virtual machine functions
  */
#ifndef _VM__H_
#define _VM__H_

#include "bitmaps.h"

/** \brief Memory size in words */
#define RAMSIZE	65536           //FIXME: is this the place?
/** \brief Memory page size for indirection and swapping */
#define PAGESIZE 512

/** \brief Virtual machine state descriptor */
struct t_virtual_machine
{
/** \brief Data stack */
  uint16_t ivm_ds[16];
/** \brief Return stack */
  uint16_t ivm_rs[16];
/** \brief top of data stack */
  int8_t ivm_dp;
/** \brief top of return stack */
  int8_t ivm_rp;
/** \brief Program counter for virtual machine */
  int16_t ivm_pc;
/** \brief Ram pages bitmap */
  struct t_bitmap ram_bitmap;
/** \brief Ram pages pointers no need to save the pointers, but save the contents */
  uint16_t *ram_pointers[RAMSIZE / PAGESIZE];
};



#endif 
