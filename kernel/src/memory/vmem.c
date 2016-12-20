#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

static PTE vptable[/*(VMEM_ADDR + SCR_SIZE) / PAGE_SIZE + 1*/NR_PTE] align_to_page;

/* Use the function to get the start address of user page directory. */
inline PDE* get_updir();

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
//	panic("please implement me");
	PDE *pdir = get_updir();
	PTE *ptable = /*(PTE *)va_to_pa*/(vptable + (VMEM_ADDR >> 12));
/*	int pdir_idx;
	for(pdir_idx = 0; pdir_idx < SCR_SIZE / PT_SIZE; pdir_idx ++) {
		pdir[pdir_idx].val = make_pde(ptable);
	}
*/
	pdir[0].val = make_pde(va_to_pa(ptable));
	int pframe_addr;
//	for(pframe_addr = 0; pframe_addr < VMEM_ADDR; pframe_addr += PAGE_SIZE) ptable ++;
	for(pframe_addr = VMEM_ADDR; pframe_addr < VMEM_ADDR + SCR_SIZE + PAGE_SIZE; pframe_addr += PAGE_SIZE) {
		ptable->val = make_pte(pframe_addr);
		ptable ++;
	}
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)(VMEM_ADDR);
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
//		assert(buf[SCR_SIZE / 4 - 1] == SCR_SIZE / 4 - 1);
	for(i = 0; i < SCR_SIZE / 4; i ++) {
//		if( buf[i] != i ) Log("%d %d", i, buf[i]);	
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

