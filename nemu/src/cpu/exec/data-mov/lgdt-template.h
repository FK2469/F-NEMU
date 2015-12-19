#include "cpu/exec/template-start.h"

#define instr lgdt

make_helper(concat(lgdt_, SUFFIX)) {
	lnaddr_t addr = instr_fetch(eip + 2, 4);
	cpu.GDTR.limit = lnaddr_read(addr, 2);
	cpu.GDTR.base = lnaddr_read(addr + 2, 4);
	print_asm("lgdt" str(SUFFIX) " 0x%x", addr); 
	return 6;
}
#include "cpu/exec/template-end.h"
