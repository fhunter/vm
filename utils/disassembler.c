#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

char *ops[] = {
  "T   ",
  "N   ",
  "T+N ",
  "T&N ",
  "T|N ",
  "T^N ",
  "~T  ",
  "N=T ",
  "N<T ",
  "N>>T",
  "T-1 ",
  "R   ",
  "[T] ",
  "N<<T",
  "dpth",
  "Nu<T",
};

int main( int argc, char **argv )
{
  uint16_t buffer[8192] = { 0 };
  uint16_t length = 0;
  uint16_t i;
  uint16_t code;
  length = read( 0, buffer, sizeof( buffer ) );
  length >>= 1;                 //uint16
  for( i = 0; i < length; i++ ) {
    printf( "%04x\t", i );
    code = buffer[i];
    if( code & 0x8000 ) {
      //literal
      printf( "lit %04x (%d)", code & 0x7fff, code & 0x7fff );
    }
    else {
      switch ( code >> 13 ) {
        case 0:
          {
            printf( "jump %04x", code & 0x1fff );
            break;
          }
        case 1:
          {
            printf( "jmpc %04x", code & 0x1fff );
            break;
          }
        case 2:
          {
            printf( "call %04x", code & 0x1fff );
            break;
          }
        case 3:
          {
            int rs = ( code & 0x0c ) >> 2;
            int ds = ( code & 0x03 );
            rs = ( rs & 0x02 ) ? rs | ( -3 ) : rs;
            ds = ( ds & 0x02 ) ? ds | ( -3 ) : ds;
            printf( "alu  %s T'=%s %s %s %s rstack+=%+d dstack+=%+d",
                    code & ( 1 << 12 ) ? "r->pc" : "     ",
                    ops[( code >> 8 ) & 0x0f],
                    ( code & ( 1 << 7 ) ) ? "T->N" : "    ",
                    ( code & ( 1 << 6 ) ) ? "T->R" : "    ",
                    ( code & ( 1 << 5 ) ) ? "N->[T]" : "      ",
                    rs, ds );
            break;
          };
        default:
          {
            printf( "unknown: %04x", code );
          }
      };
    };
    printf( "\n" );
  }
  return 0;
};
