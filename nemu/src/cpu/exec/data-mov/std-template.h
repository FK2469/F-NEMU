#include "cpu/exec/template-start.h"

#define instr std

static void do_execute() {
	cpu.DF = 1;
	print_asm("std");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
