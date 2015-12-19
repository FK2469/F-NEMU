#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute() {
	cpu.esp = cpu.ebp;
	DATA_TYPE result = swaddr_read(cpu.esp, DATA_BYTE,R_SS);
	cpu.esp += DATA_BYTE;
	cpu.ebp = result;
	print_asm("leave");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
