#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	cpu.OF = cpu.CF = 0;
	cpu.ZF = (op_src->val == op_dest->val);
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
