#include "cpu/exec/template-start.h"

#define instr movsr

make_helper(concat(movsr_rm2cr_, SUFFIX)) {
	uint8_t r = instr_fetch(eip + 1, 1);
	cpu.cr._[(r >> 3) & 0x7] = REG(r & 0x7);
	print_asm("movsr" str(SUFFIX) " %%%s,%%%s", REG_NAME(r & 0x7), "cr0");
	return 2;
}

make_helper(concat(movsr_cr2rm_, SUFFIX)) {
	uint8_t r = instr_fetch(eip + 1, 1);
	REG(r & 0x7) = cpu.cr._[(r >> 3) & 0x7];
	print_asm("movsr" str(SUFFIX) " %%%s,%%%s", "cr0", REG_NAME(r & 0x7));
	return 2;
}

#include "cpu/exec/template-end.h"
