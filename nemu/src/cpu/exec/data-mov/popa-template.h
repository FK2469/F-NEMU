#include "cpu/exec/template-start.h"

#define instr popa

static void do_execute() {
	int len = 4, i;
	for(i = 7; i >= 0; -- i) {
		if(i != R_ESP) cpu.gpr[i]._32 = swaddr_read(cpu.esp, len, R_SS);
		cpu.esp += len;
	}
	print_asm("popa");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
