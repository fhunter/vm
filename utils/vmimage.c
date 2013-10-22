#include "state.h"

int main( int argc, char **argv )
{
	struct t_virtual_machine vm;
	ivm_reset(vm);
	vm_save_state(stdout, vm);
	return 0;
};
