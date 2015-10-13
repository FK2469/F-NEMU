#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	cpu.eip += op_src->val;
//	printf("%x\n",cpu.eip);
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
