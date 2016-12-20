#include "cpu/exec/template-start.h"

#define instr jmp


static void do_execute() {
	int len = instr_len();
	if(op_src->type == OP_TYPE_IMM) {
		cpu.eip += op_src->val;
		snprintf(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + len + 1);
	} else {
		cpu.eip = op_src->val - len - 1;
	}
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

void load_sreg(uint32_t);

make_helper(concat(ljmp_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	uint16_t sreg = instr_fetch(eip + 5, 2);
	cpu.cs = sreg;
	load_sreg(R_CS);
//	printf("%x\n", cpu.cs);
	cpu.eip = addr - 7;
	print_asm("ljmp" str(SUFFIX) " $0x%x,$0x%x", sreg, addr);
	return 7;
}

#include "cpu/exec/template-end.h"
