#include "cpu/exec/template-start.h"

#define instr cmps

static void do_execute() {
	op_src->type = op_dest->type = OP_TYPE_REG;
	op_src->reg = R_ESI; op_dest->reg = R_EDI;
	snprintf(op_src->str, 11, "%%es:(%%edi)");
	snprintf(op_dest->str, 11, "%%ds:(%%esi)");
	op_src->val = swaddr_read(cpu.edi, DATA_BYTE, R_ES);
	op_dest->val = swaddr_read(cpu.esi, DATA_BYTE, R_DS);

	DATA_TYPE result = op_dest->val - op_src->val;
	if(op_dest->val < op_src->val) cpu.CF = 1; else cpu.CF = 0;
	if(MSB(op_dest->val) != MSB(op_src->val) && MSB(result) != MSB(op_dest->val))
		cpu.OF = 1; else cpu.OF = 0;
	Updata_EFLAGS(result);
	cpu.edi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	cpu.esi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
