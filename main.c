#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include "launcher.h"
#include "ivm.h"

uint16_t ram[65536];

static void ivm_mem_put(uint16_t addr, uint16_t value)
{
	if(addr<0xfc00){
		//Access RAM
		ram[addr]=value;
	} else
	{
		//Access peripherials
	}
};
static uint16_t ivm_mem_get(uint16_t addr)
{
	if(addr<0xfc00){
		//Access RAM
		return ram[addr];
	} else
	{
		//Access peripherials
		return 0;
	}
};

int main( int argc, char **argv )
{
  int i = 0;
  i = read(STDIN_FILENO, ram, sizeof(ram));
  printf("loaded %d bytes\n",i);
  ivm_reset();
  i = 0;
  while( 1 ) {
    ivm_step(ram[ivm_pc]);
    printf("i=%04x ivm_pc=%04x\tivm_dp=%08x\tivm_rs=%08x\n",i,ivm_pc,ivm_dp,ivm_rs);
    usleep(100000);
    i++;
  };
  return 0;
};
