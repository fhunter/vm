/** \file lcd.c
  * \author Nikolay Zamotaev
  */
#include "lcd.h"

void lcd_init(  )
{
};

void lcd_done(  )
{
};

void lcd_fill( uint8_t colour )
{
};

void lcd_fillarea( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                   uint8_t colour )
{
};

void lcd_putpixel( uint16_t x, uint16_t y, uint8_t colour )
{
};

void lcd_fillarea_a( uint16_t x1, uint16_t y1, uint16_t x2,
                     uint16_t y2, uint8_t * ram, uint16_t length )
{
};

uint8_t lcd_getpixel( uint16_t x, uint16_t y )
{
  return 0;
};

void lcd_getarea( uint16_t x1, uint16_t y1, uint16_t x2,
                  uint16_t y2, uint8_t * ram, uint16_t length )
{

};
