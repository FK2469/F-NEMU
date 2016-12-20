#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
	op_src->type = op_dest->type = OP_TYPE_REG;
	op_src->reg = R_ESI; op_dest->reg = R_EDI;
	snprintf(op_src->str, 11, "%%ds:(%%esi)");
	snprintf(op_dest->str, 11, "%%es:(%%edi)");

	swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE, R_DS), R_ES);
	cpu.edi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	cpu.esi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
