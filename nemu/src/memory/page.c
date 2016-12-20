#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"

typedef union {
	struct {
		uint32_t offset	:	12;
		uint32_t page	:	10;
		uint32_t dir	:	10;
	};
	uint32_t val;
} lnaddr_st;

static uint32_t cr3 = 0;
#define limit 4096
uint32_t hwaddr_read(lnaddr_t, size_t);
hwaddr_t tlb_read(lnaddr_t, uint32_t);
void init_tlb();

PTE page_read(lnaddr_t addr, uint32_t len) {
	lnaddr_st lnaddr;
	lnaddr.val = addr;
	
	PDE dir_entry;
	dir_entry.val = 
		hwaddr_read((cpu.cr._3.page_directory_base << 12) + 4 * lnaddr.dir, 4);
//	Assert(lnaddr.offset + len <= limit, "Cross page! %x %x", lnaddr.offset, len);
	Assert(dir_entry.present == 1, "dir_entry is not valid!  0x%x", addr);

	PTE pg_tbl_entry;
	pg_tbl_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
	Assert(pg_tbl_entry.present == 1, "pg_tbl_entry is not valid!  0x%x %x %x", 
			addr, dir_entry.page_frame, pg_tbl_entry.val);

//	hwaddr_t hwaddr = (pg_tbl_entry.page_frame << 12) + lnaddr.offset;
	return pg_tbl_entry;
}

hwaddr_t page_translate(lnaddr_t addr, uint32_t len) {
	if(cr3 != cpu.cr._[3]) {
		init_tlb();
		cr3 = cpu.cr._[3];
//		printf("^_^%x\n", cr3);
	}
	return tlb_read(addr, len);
}
