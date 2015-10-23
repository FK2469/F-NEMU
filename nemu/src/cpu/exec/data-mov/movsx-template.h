#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
//	printf("%d %d\n", op_src->size, DATA_BYTE);
//	printf("%x\n", op_src->val);
	if(op_src->val >> (8 * op_src->size - 1) == 1)
		op_src->val |= ((1 << 8 * (DATA_BYTE - op_src->size))-1) << 8 * op_src->size;else 
			op_src->val &= ((1 << 8 * op_src->size)-1);
//	printf("%x\n", op_src->val);
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm_b2r)
#endif

#if DATA_BYTE == 4 
make_instr_helper(rm_w2r)
#endif

#include "cpu/exec/template-end.h"
