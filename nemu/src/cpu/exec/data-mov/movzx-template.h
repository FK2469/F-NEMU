#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	DATA_TYPE result = op_src->val & ((1ll << (op_src->size * 8)) - 1);
//	printf("%x %x %d\n", result, op_src->val, op_src->size);
	OPERAND_W(op_dest, result);
//	printf("%x\n", swaddr_read(op_dest->addr, DATA_BYTE));
	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm_b2r)
#endif

#if DATA_BYTE == 4 
make_instr_helper(rm_w2r)
#endif

#include "cpu/exec/template-end.h"
