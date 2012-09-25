#include <stdio.h>
#include <strings.h>
#include "launcher.h"
#include "dcpu16.h"

int main( int argc, char **argv )
{
//  hardware_init(  );            //Here initialise either hardware or emulator
  context_t context;
  memset( &context, 0, sizeof( context ) );
  while( 1 ) {
    process_one_instruction( &context );
    print_status( &context );
//    launcher(  );
  };
  return 0;
};
