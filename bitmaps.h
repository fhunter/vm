#ifndef _BITMAPS__H_
#define _BITMAPS__H_

/** \file bitmaps.h
  * \author Nikolay Zamotaev
  * \brief Bit Array operations
  */

#include <stdint.h>

void set_bit( uint8_t * map, uint16_t addr );
void clr_bit( uint8_t * map, uint16_t addr );
uint8_t get_bit( uint8_t * map, uint16_t addr );

#endif
