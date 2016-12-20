#include "cpu/exec/template-start.h"

#define instr pusha

static void do_execute() {
	int len = 4, i, tmp = cpu.esp;
	for(i = 0; i < 8; ++ i) {
		cpu.esp -= len;
		swaddr_write(cpu.esp, len, i == R_ESP ? tmp : cpu.gpr[i]._32, R_SS);
	}
	print_asm("pusha");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
