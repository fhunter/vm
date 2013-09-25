/** \file lcd.h
  * \author Nikolay Zamotaev
  */
#ifndef _LCD__H_
#define _LCD__H_
#include <stdint.h>

void lcd_fill( uint8_t colour );
void lcd_fillarea( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                   uint8_t colour );
void lcd_putpixel( uint16_t x, uint16_t y, uint8_t colour );
void lcd_fillarea_a( uint16_t x1, uint16_t y1, uint16_t x2,
                     uint16_t y2, uint8_t * ram, uint16_t length );
uint8_t lcd_getpixel( uint16_t x, uint16_t y );
void lcd_getarea( uint16_t x1, uint16_t y1, uint16_t x2,
                  uint16_t y2, uint8_t * ram, uint16_t length );

#endif
