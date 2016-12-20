#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"

uint32_t lnaddr_read(hwaddr_t, size_t);
uint32_t hwaddr_read(hwaddr_t, size_t);
uint32_t hwaddr_write(hwaddr_t, size_t, uint32_t);

void load_sreg(uint32_t sreg) {
	uint8_t tmp[8]; 
	int i;
	for(i = 0; i < 8; ++ i) 
		tmp[i] = lnaddr_read(cpu.GDTR.base + cpu.sr[sreg].index * 8 + i, 1);
	SegDesc *segdesc = (SegDesc*)tmp;
	Assert(segdesc->present == 1, "Segdesc is not valid! 0x%x", cpu.GDTR.base + cpu.sr[sreg].index * 8);
	Assert(cpu.sr[sreg].index * 8 < (segdesc->limit_19_16 << 16) + segdesc->limit_15_0, "Segment overflow!");
	cpu.sr[sreg].invi = *segdesc;
}

lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg) {
	SegDesc *segdesc = &cpu.sr[sreg].invi;
	return 
		(segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + 
		segdesc->base_15_0 + addr;
}

void init_seg() {
	uint8_t tmp[8];	
	cpu.cs = 8;
	SegDesc *segdesc = (SegDesc*)tmp;
	segdesc->present = 1;
	segdesc->base_31_24 = segdesc->base_23_16 = segdesc->base_15_0 = 0;
	segdesc->limit_19_16 = segdesc->limit_15_0 = -1;
	cpu.sr[R_SS].invi = *segdesc;
	cpu.cr._[0] = cpu.cr._[3] = 0;
}
