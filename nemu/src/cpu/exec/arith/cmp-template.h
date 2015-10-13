#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	DATA_TYPE result = op_dest->val - op_src->val;
	if(op_dest->val < op_src->val) cpu.CF = 1; else cpu.CF = 0;
	if(MSB(op_dest->val) != MSB(op_src->val) && MSB(result) != MSB(op_dest->val)) 
		cpu.OF = 1; else cpu.OF = 0;
	Updata_EFLAGS(result);
	print_asm_template2();
}

make_instr_helper(si2rm)

#include "cpu/exec/template-end.h"
