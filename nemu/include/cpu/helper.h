#ifndef __HELPER_H__
#define __HELPER_H__

#include "nemu.h"
#include "cpu/decode/operand.h"

/* All function defined with 'make_helper' return the length of the operation. */
#define make_helper(name) int name(swaddr_t eip)

extern Operands ops_decoded;

#define op_src (&ops_decoded.src)
#define op_src2 (&ops_decoded.src2)
#define op_dest (&ops_decoded.dest)

static inline uint32_t instr_fetch(swaddr_t addr, size_t len) {
	return swaddr_read(addr, len, R_CS);
}

static int len;

/* Instruction Decode and EXecute */
static inline int idex(swaddr_t eip, int (*decode)(swaddr_t), void (*execute) (void)) {
	/* eip is pointing to the opcode */
	len = decode(eip + 1);
//	cpu.eip += len + 1;
//	printf("idex\n");
	execute();
	return len + 1;	// "1" for opcode
}

static inline int instr_len() {
	return len;
}

/* shared by all helper function */


#endif
