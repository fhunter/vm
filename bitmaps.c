/** \file bitmaps.c
  * \author Nikolay Zamotaev
  * \brief Bit Array operations
  */
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

/** \brief Sets bit in bitmap
  * \param map -- pointer to bitmap
  * \param addr -- address for which to set the bit in bitmap
  * \todo Implement
  */
void set_bit( uint8_t * map, uint16_t addr )
{
  uint32_t temp = 0;
  temp = addr >> pagesize2bits( 256 );
  map[temp >> 3] |= ( 1 << ( temp & 0x07u ) );
};

/** \brief Clears bit in bitmap
  * \param map -- pointer to bitmap
  * \param addr -- address for which to clear the bit in bitmap
  * \todo Implement
  */
void clr_bit( uint8_t * map, uint16_t addr )
{
  uint32_t temp = 0;
  temp = addr >> pagesize2bits( 256 );
  map[temp >> 3] &= ~( 1 << ( temp & 0x07u ) );
};

/** \brief Checks if a bit in bitmap is set
  * \param map -- pointer to bitmap
  * \param addr -- address for which to check the bit in bitmap
  * \return 0 -- if the bit is not set
  * \return non-zero -- if the bit is set
  * \todo Implement
  */
uint8_t get_bit( uint8_t * map, uint16_t addr )
{
  uint32_t temp = 0;
  temp = addr >> pagesize2bits( 256 );
  return map[temp >> 3] | ( 1 << ( temp & 0x07u ) );
};
