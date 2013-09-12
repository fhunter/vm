#ifndef _BITMAPS__H_
#define _BITMAPS__H_

/** \file bitmaps.h
  * \author Nikolay Zamotaev
  * \brief Bit Array operations
  */

#include <stdint.h>

/** \brief Calculates bitmap size for the ramsize and page size
  * \param ramsize -- memory size, to calculate bitmap for
  * \param pagesize -- memory page size
  * \return size of bitmap in bytes
  */
#define get_bitmap_size( ramsize, pagesize )	((ramsize)/(pagesize)/8)

void clr_allbits( uint8_t * map );
void set_bit( uint8_t * map, uint16_t addr );
void clr_bit( uint8_t * map, uint16_t addr );
uint8_t get_bit( uint8_t * map, uint16_t addr );

#endif
