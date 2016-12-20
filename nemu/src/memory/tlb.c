#include "common.h"
#include "burst.h"
#include "rand.h"
#include "misc.h"
#include "../../lib-common/x86-inc/mmu.h"

#define COL_WIDTH 12
#define LINE_WIDTH 6
#define FLAG_WIDTH (32 - COL_WIDTH)

#include "tlb.h"

tlb_t tlb;

uint32_t rand(int);
PTE page_read(lnaddr_t, uint32_t);

void init_tlb() {
	memset(tlb.valid, 0, sizeof tlb.valid);
}

hwaddr_t tlb_read(lnaddr_t addr, uint32_t len) {
	tlb_addr temp;
	temp.addr = addr;
	uint32_t col = temp.col;
	uint32_t flag = temp.flag;
	uint32_t line, line_ = 0;
	bool full = true;
	for(line = 0; line < NR_LINE; ++ line) {
		if(tlb.valid[line]) {
			if(tlb.flag[line] == flag) {
				return (tlb.data[line].page_frame << 12) + col;
			}
		} else {
			full = false;
			line_ = line;
		}
	}
	if(full) line_ = rand(addr) & LINE_MASK;
	tlb.valid[line_] = true;
	tlb.flag[line_] = flag;
	tlb.data[line_] = page_read(addr, len);
	return (tlb.data[line_].page_frame << 12) + col;
}

