#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute() {
	op_src->type = op_dest->type = OP_TYPE_REG;
	op_src->reg = R_EAX; op_dest->reg = R_EDI;
	snprintf(op_src->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
	snprintf(op_dest->str, 11, "%%es:(%%edi)");

	swaddr_write(cpu.edi, DATA_BYTE, (DATA_TYPE) cpu.eax, R_ES);
	cpu.edi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
