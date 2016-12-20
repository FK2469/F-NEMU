#include "cpu/exec/template-start.h"

extern int len;

#define if_exec() {\
	cpu.eip += op_src->val;\
	snprintf(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + len + 1);\
}

#define else_exec() snprintf(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + op_src->val + len + 1)

#define all_exec() static void do_execute() {\
				       if(JMPFLAG) if_exec() else else_exec();\
					   print_asm_template1();\
				   }\
				   make_instr_helper(si)

#define instr ja
#define JMPFLAG cpu.CF == 0 && cpu.ZF == 0
all_exec();
#undef JMPFLAG
#undef instr

#define instr jae
#define JMPFLAG cpu.CF == 0
all_exec();
#undef JMPFLAG
#undef instr

#define instr jb
#define JMPFLAG cpu.CF == 1
all_exec();
#undef JMPFLAG
#undef instr

#define instr je
#define JMPFLAG cpu.ZF == 1
all_exec();
#undef JMPFLAG
#undef instr

#define instr jbe
#define JMPFLAG cpu.ZF == 1 || cpu.CF == 1
all_exec();
#undef JMPFLAG
#undef instr

#define instr jl
#define JMPFLAG cpu.SF != cpu.OF
all_exec();
#undef JMPFLAG
#undef instr

#define instr jle
#define JMPFLAG cpu.ZF == 1 || cpu.SF != cpu.OF
all_exec();
#undef JMPFLAG
#undef instr

#define instr jg
#define JMPFLAG cpu.ZF == 0 && cpu.SF == cpu.OF
all_exec();
#undef JMPFLAG	
#undef instr

#define instr jge
#define JMPFLAG cpu.SF == cpu.OF
all_exec();
#undef JMPFLAG
#undef instr

#define instr jne
#define JMPFLAG cpu.ZF == 0
all_exec();
#undef JMPFLAG
#undef instr

#define instr jns
#define JMPFLAG cpu.SF == 0
all_exec();
#undef JMPFLAG
#undef instr

#define instr js
#define JMPFLAG cpu.SF == 1
all_exec();
#undef JMPFLAG
#undef instr

#include "cpu/exec/template-end.h"
