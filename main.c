#include <stdio.h>
#include "launcher.h"

int main( int argc, char **argv )
{
  hardware_init(  );            //Here initialise either hardware or emulator
  while( 1 ) {
    launcher(  );
  };
  return 0;
};
