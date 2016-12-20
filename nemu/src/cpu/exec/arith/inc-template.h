#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	if(MSB(op_src->val) == 0 && MSB(result) != MSB(op_src->val))
		cpu.OF = 1; else cpu.OF = 0;
	Updata_EFLAGS(result);
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
