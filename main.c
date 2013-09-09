#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include "launcher.h"
#include "ivm.h"

uint16_t ram[65536];

static void ivm_mem_put(uint16_t addr, uint16_t value)
{
	ram[addr]=value;
	if(addr<(65536/2))
	{
		printf("Written mem[%08x]=%08x\n",addr,value);
	} else
	{
		printf("Written mem[%08x]=%08x\n",addr,value);
	}
};
static uint16_t ivm_mem_get(uint16_t addr)
{
	return ram[addr];
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
