#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	cpu.esp -= 4;
	if(op_src->val < 0x100000 || 0xffffffff - op_src->val < 0x100000) {
		swaddr_write(cpu.esp, 4, cpu.eip + 4);
		cpu.eip += op_src->val;
	}else {
		swaddr_write(cpu.esp, 4, cpu.eip + 1);
		cpu.eip = op_src->val - 2;
	}
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
