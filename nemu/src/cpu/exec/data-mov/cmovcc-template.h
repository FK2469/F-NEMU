#include "cpu/exec/template-start.h"

#define if_exec() OPERAND_W(op_dest, op_src->val);

#define else_exec() ;

#define all_exec() static void do_execute() {\
				       if(CMOVFLAG) if_exec() else else_exec();\
					   print_asm_template2();\
				   }\
				   make_instr_helper(rm2r)

#define instr cmova
#define CMOVFLAG cpu.CF == 0 && cpu.ZF == 0
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovae
#define CMOVFLAG cpu.CF == 0
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovb
#define CMOVFLAG cpu.CF == 1
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmove
#define CMOVFLAG cpu.ZF == 1
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovbe
#define CMOVFLAG cpu.ZF == 1 || cpu.CF == 1
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovl
#define CMOVFLAG cpu.SF != cpu.OF
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovle
#define CMOVFLAG cpu.ZF == 1 || cpu.SF != cpu.OF
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovg
#define CMOVFLAG cpu.ZF == 0 && cpu.SF == cpu.OF
all_exec();
#undef CMOVFLAG	
#undef instr

#define instr cmovge
#define CMOVFLAG cpu.SF == cpu.OF
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovne
#define CMOVFLAG cpu.ZF == 0
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovns
#define CMOVFLAG cpu.SF == 0
all_exec();
#undef CMOVFLAG
#undef instr

#define instr cmovs
#define CMOVFLAG cpu.SF == 1
all_exec();
#undef CMOVFLAG
#undef instr

#include "cpu/exec/template-end.h"
