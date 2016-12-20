#include "cpu/exec/template-start.h"

#define if_exec() OPERAND_W(op_src, 1);

#define else_exec() OPERAND_W(op_src, 0)

#define all_exec() static void do_execute() {\
				       if(SETFLAG) if_exec() else else_exec();\
					   print_asm_template1();\
				   }\
				   make_instr_helper(rm)

#define instr seta
#define SETFLAG cpu.CF == 0 && cpu.ZF == 0
all_exec();
#undef SETFLAG
#undef instr

#define instr setae
#define SETFLAG cpu.CF == 0
all_exec();
#undef SETFLAG
#undef instr

#define instr setb
#define SETFLAG cpu.CF == 1
all_exec();
#undef SETFLAG
#undef instr

#define instr sete
#define SETFLAG cpu.ZF == 1
all_exec();
#undef SETFLAG
#undef instr

#define instr setbe
#define SETFLAG cpu.ZF == 1 || cpu.CF == 1
all_exec();
#undef SETFLAG
#undef instr

#define instr setl
#define SETFLAG cpu.SF != cpu.OF
all_exec();
#undef SETFLAG
#undef instr

#define instr setle
#define SETFLAG cpu.ZF == 1 || cpu.SF != cpu.OF
all_exec();
#undef SETFLAG
#undef instr

#define instr setg
#define SETFLAG cpu.ZF == 0 && cpu.SF == cpu.OF
all_exec();
#undef SETFLAG	
#undef instr

#define instr setge
#define SETFLAG cpu.SF == cpu.OF
all_exec();
#undef SETFLAG
#undef instr

#define instr setne
#define SETFLAG cpu.ZF == 0
all_exec();
#undef SETFLAG
#undef instr

#define instr setns
#define SETFLAG cpu.SF == 0
all_exec();
#undef SETFLAG
#undef instr

#define instr sets
#define SETFLAG cpu.SF == 1
all_exec();
#undef SETFLAG
#undef instr

#include "cpu/exec/template-end.h"
