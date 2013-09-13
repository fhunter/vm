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
/** \brief Structure describing ram page residency bitmap
  */
struct t_bitmap
{
  /** \brief RAM size argument */
  uint32_t ramsize;
  /** \brief RAM page size argument, must be power of 2 */
  uint32_t pagesize;
  /** \brief Bitmap itself. Must be allocated by bitmap_create */
  unsigned int *bitmap;
};

struct t_bitmap bitmap_create( uint32_t ramsize, uint32_t pagesize );
void bitmap_destroy(struct t_bitmap map);
void bitmap_clearall( struct t_bitmap map );
void bitmap_set( struct t_bitmap map, uint16_t addr );
void bitmap_clr( struct t_bitmap map, uint16_t addr );
uint8_t bitmap_get( struct t_bitmap map, uint16_t addr );

#endif
