#include "cpu/exec/template-start.h"

#define instr ja

static void do_execute() {
	if(cpu.CF == 0 && cpu.ZF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jae

static void do_execute() {
	if(cpu.CF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jb

static void do_execute() {
	if(cpu.CF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jbe

static void do_execute() {
	if(cpu.ZF == 1 || cpu.CF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jc

static void do_execute() {
	if(cpu.CF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr


#define instr je

static void do_execute() {
	if(cpu.ZF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jz

static void do_execute() {
	if(cpu.ZF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jg

static void do_execute() {
	if(cpu.ZF == 0 && cpu.SF == cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jge

static void do_execute() {
	if(cpu.SF == cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jl

static void do_execute() {
	if(cpu.SF != cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr


#define instr jle

static void do_execute() {
	if(cpu.ZF == 1 || cpu.SF != cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jna

static void do_execute() {
	if(cpu.ZF == 1 || cpu.CF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnae

static void do_execute() {
	if(cpu.CF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnb

static void do_execute() {
	if(cpu.CF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnbe

static void do_execute() {
	if(cpu.ZF == 0 && cpu.CF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnc

static void do_execute() {
	if(cpu.CF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jne

static void do_execute() {
	if(cpu.ZF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jng

static void do_execute() {
	if(cpu.ZF == 1||cpu.SF != cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnge

static void do_execute() {
	if(cpu.SF != cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnl

static void do_execute() {
	if(cpu.SF == cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnle

static void do_execute() {
	if(cpu.ZF == 0 && cpu.SF == cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jno

static void do_execute() {
	if(cpu.OF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnp

static void do_execute() {
	if(cpu.PF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jns

static void do_execute() {
	if(cpu.SF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jnz

static void do_execute() {
	if(cpu.ZF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jo

static void do_execute() {
	if(cpu.OF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jp

static void do_execute() {
	if(cpu.PF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jpe

static void do_execute() {
	if(cpu.PF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jpo

static void do_execute() {
	if(cpu.PF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr js

static void do_execute() {
	if(cpu.SF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#if DATA_BYTE == 1
int concat(jcxz_si_, SUFFIX)(swaddr_t eip){
    int len = concat(decode_i_, SUFFIX)(eip + 1);
    len += 1;
    if (reg_w(R_ECX) == 0){
        DATA_TYPE_S val0 = op_src -> val;
        int val = val0;
        return len + val;
    }
    return len;
}
int concat(jecxz_si_, SUFFIX)(swaddr_t eip){
    int len = concat(decode_i_, SUFFIX)(eip + 1);
    len += 1;
    if (reg_l(R_ECX) == 0){
        DATA_TYPE_S val0 = op_src -> val;
        int val = val0;
        return len + val;
    }
    return len;
}
#endif

#include "cpu/exec/template-end.h"
