#include "cpu/exec/template-start.h"

#define instr cwtl

static void do_execute() {
//	OPERAND_W(op_dest, op_src->val);
	if((cpu.eax >> (DATA_BYTE * 4 - 1)) != 0)
		cpu.eax |= ((1ll << (DATA_BYTE * 4)) - 1) << (DATA_BYTE * 4); 
	else cpu.eax &= (1ll << (DATA_BYTE * 4)) - 1;
	print_asm(DATA_BYTE == 4 ? "cwtll" : "cwtlw");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
