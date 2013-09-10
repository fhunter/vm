#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include "launcher.h"
#include "ivm.h"

#define RAMSIZE	65536
#define PAGESIZE 256

uint16_t ram[65536];
uint8_t ram_bitmap[RAMSIZE/PAGESIZE/(sizeof(uint8_t)*8)];

static void ivm_mem_put(uint16_t addr, uint16_t value)
{
	if(addr<0xfc00){
		//Access RAM
		ram[addr]=value;
		//Update bitmap
//		set_bit(ram_bitmap,addr);
	} else
	{
		if(addr>0xff00){
			printf("\033[%d;%dH%c",(0xffff-addr)/16,(0xffff-addr)%16,value);
			//Video RAM
		}
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
    printf("\033[18;1H i=%04x ivm_pc=%04x\tivm_dp=%08x\tivm_rs=%08x\n",i,ivm_pc,ivm_dp,ivm_rs);
    usleep(100000);
    i++;
  };
  return 0;
};
