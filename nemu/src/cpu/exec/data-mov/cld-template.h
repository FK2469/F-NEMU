#include "cpu/exec/template-start.h"

#define instr cld

static void do_execute() {
	cpu.DF = 0;
	print_asm("ret"); 
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
