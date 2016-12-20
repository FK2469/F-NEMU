#include "cpu/exec/template-start.h"

#define instr movsp

make_helper(concat(movsp_rm2cr_, SUFFIX)) {
	uint8_t r = instr_fetch(eip + 1, 1);
	cpu.cr._[(r >> 3) & 0x7] = REG(r & 0x7);
	print_asm("movsp" str(SUFFIX) " %%%s,%%cr%d", REG_NAME(r & 0x7), (r >> 3) & 0x7);
	return 2;
}

make_helper(concat(movsp_cr2rm_, SUFFIX)) {
	uint8_t r = instr_fetch(eip + 1, 1);
	REG(r & 0x7) = cpu.cr._[(r >> 3) & 0x7];
	print_asm("movsp" str(SUFFIX) " %%cr%d,%%%s", (r >> 3) & 0x7, REG_NAME(r & 0x7));
	return 2;
}

#include "cpu/exec/template-end.h"
