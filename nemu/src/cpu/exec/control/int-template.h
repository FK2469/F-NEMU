#include "cpu/exec/template-start.h"

#define instr int

void raise_intr(uint8_t/*, uint32_t*/);

static void do_execute() {
	cpu.eip += instr_len();
	raise_intr(op_src->val/*, instr_len()*/);
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
