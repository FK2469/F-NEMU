#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	DATA_TYPE result = op_src->val & op_dest->val;
	cpu.OF = cpu.CF = 0;
	Updata_EFLAGS(result);
	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"
