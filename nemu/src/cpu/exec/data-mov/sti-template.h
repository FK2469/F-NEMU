#include "cpu/exec/template-start.h"

#define instr sti

static void do_execute() {
	cpu.IF = 1;
	print_asm("sti");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
