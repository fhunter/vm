/** \file bitmaps.c
  * \author Nikolay Zamotaev
  * \brief Bit Array operations
  */
#include <stdlib.h>
#include <limits.h>
#include "bitmaps.h"

/** \brief Converts from page size to number of bits to address contents of the page
  * \param pagesize -- size of the page
  * \return number of bits for addressing page contents
  * \note pagesize must be a power of 2
  * \note pagesize must be lesser than 2^32
  */
static uint8_t pagesize2bits( uint16_t pagesize )
{
  uint8_t i;
  for( i = 0; i < 32; i++ ) {
    if( pagesize == 0 )
      return i;
    pagesize >>= 1;
  };
  return 32;
};

#define INT_BIT	(sizeof(unsigned int)*CHAR_BIT)
#define BITMASK( b )		(1<<((b)%INT_BIT))
#define BITSLOT( b )		((b)/INT_BIT)
#define BITSET( a, b )		((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR( a, b )	((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST( a, b )		((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS( nb )		(( nb + INT_BIT - 1 )/INT_BIT)

/** \brief Creates bitmap and allocates memory for it
  * \param ramsize -- RAM memory size for bitmap
  * \param pagesize -- RAM page size, must be a power of 2
  * \return Returns initiated bitmap structure.
  * \note Structure part is allocated by malloc(), if must be disposed off by bitmap_destroy()
  */
struct t_bitmap bitmap_create( uint32_t ramsize, uint32_t pagesize )
{
  struct t_bitmap map;
  map.ramsize = ramsize;
  map.pagesize = pagesize;
  map.bitmap =
    calloc( BITNSLOTS( ( ramsize + pagesize ) / pagesize ),
            sizeof( unsigned int ) );
  return map;
}

/** \brief Frees the memory allocated by bitmap_create()
  * \param map -- ram map to free
  * \pre bitmap_create()
  */
void bitmap_destroy( struct t_bitmap map )
{
  if( map.bitmap != NULL )
    free( map.bitmap );
};

/** \brief Sets bit in bitmap
  * \param map -- pointer to bitmap
  * \param addr -- address for which to set the bit in bitmap
  * \pre bitmap_create must be called to create map
  */
void bitmap_set( struct t_bitmap map, uint16_t addr )
{
  uint32_t temp = 0;
  temp = addr >> pagesize2bits( map.pagesize );
  BITSET( map.bitmap, temp );
};

/** \brief clear all bitmap
  * \param map -- bitmap to clear
  * \pre bitmap_create must be called to create map
  */
void bitmap_clearall( struct t_bitmap map )
{
  int i;
  for( i = 0;
       i < BITNSLOTS( ( map.ramsize + map.pagesize ) / map.pagesize );
       i++ ) {
    map.bitmap[i] = 0;
  };
};

/** \brief Clears bit in bitmap
  * \param map -- bitmap
  * \param addr -- address for which to clear the bit in bitmap
  * \pre bitmap_create must be called to create map
  */
void bitmap_clr( struct t_bitmap map, uint16_t addr )
{
  uint32_t temp = 0;
  temp = addr >> pagesize2bits( map.pagesize );
  BITCLEAR( map.bitmap, temp );
};

/** \brief Checks if a bit in bitmap is set
  * \param map -- bitmap
  * \param addr -- address for which to check the bit in bitmap
  * \pre bitmap_create must be called to create map
  * \return 0 -- if the bit is not set
  * \return non-zero -- if the bit is set
  */
uint8_t bitmap_get( struct t_bitmap map, uint16_t addr )
{
  uint32_t temp = 0;
  temp = addr >> pagesize2bits( map.pagesize );
  return BITTEST( map.bitmap, temp );
};
