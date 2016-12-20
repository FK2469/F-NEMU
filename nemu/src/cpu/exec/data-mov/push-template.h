#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	int len;
	if(DATA_BYTE == 2)len = 2; else len = 4;
//	if(DATA_BYTE == )
	cpu.esp -= len;
	swaddr_write(cpu.esp, len, op_src->val, R_SS);
	print_asm_template1();
}

make_instr_helper(rm)
make_instr_helper(r)
make_instr_helper(i)

#include "cpu/exec/template-end.h"
