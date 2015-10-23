#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
//	printf("%x %d\n", op_src->val, DATA_BYTE);
	if(DATA_BYTE == 4 && op_src->val>0x100000) {
		op_src->val -= cpu.eip + 2;
		printf("*****\n");
	}
	cpu.eip += op_src->val;
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
