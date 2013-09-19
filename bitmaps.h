#ifndef _BITMAPS__H_
#define _BITMAPS__H_

/** \file bitmaps.h
  * \author Nikolay Zamotaev
  * \brief Bit Array operations
  */

#include <stdio.h>
#include <stdint.h>

/** \brief Structure holding bitmap and it's parameters
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
void bitmap_destroy( struct t_bitmap map );
void bitmap_clearall( struct t_bitmap map );
void bitmap_set( struct t_bitmap map, uint16_t addr );
void bitmap_clr( struct t_bitmap map, uint16_t addr );
uint8_t bitmap_get( struct t_bitmap map, uint16_t addr );
void save_bitmap( struct t_bitmap *map, FILE * state_file );
void load_bitmap( struct t_bitmap *map, FILE * state_file );

#endif
