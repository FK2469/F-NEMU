#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"
#include <setjmp.h>
extern jmp_buf jbuf;

uint32_t lnaddr_read(lnaddr_t, size_t);
void swaddr_write(swaddr_t, size_t, uint32_t, uint8_t);

void load_sreg(uint32_t);

void raise_intr(uint8_t NO/*, uint32_t len*/) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 *	 * That is, use ``NO'' to index the IDT.
	 *		 */
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.EFLAGS, R_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.cs, R_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip, R_SS);
	
	uint8_t tmp[8];
	int i;
	for(i = 0; i < 8; ++ i) tmp[i] = lnaddr_read(cpu.IDTR.base + NO * 0x8 + i, 1);
	GateDesc *gde = (GateDesc*)tmp;
	cpu.cs = gde->segment;
	load_sreg(R_CS);
	cpu.eip = (gde->offset_31_16 << 16) + gde->offset_15_0;
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}
