#include "cpu/exec/template-start.h"

#define instr cltd

static void do_execute() {
//	OPERAND_W(op_dest, op_src->val);
	if((cpu.eax >> (DATA_BYTE * 8 - 1)) != 0) 
		cpu.edx = (1ll << (DATA_BYTE * 8)) - 1; else cpu.edx = 0x0;
	print_asm(DATA_BYTE == 4 ? "cltdl" : "cltdw");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
