#include "common.h"
#include "cpu/reg.h"

uint32_t cache_read(hwaddr_t, size_t);

void cache_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	cache_write(addr, len, data);
}

hwaddr_t page_translate(lnaddr_t);

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	hwaddr_t hwaddr;
	if(cpu.cr._0.paging == 1) hwaddr = page_translate(addr);
		else hwaddr = addr;
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_t hwaddr;
	if(cpu.cr._0.paging == 1) hwaddr = page_translate(addr);
		else hwaddr = addr;
	hwaddr_write(hwaddr, len, data);
}

lnaddr_t seg_translate(swaddr_t, uint8_t);

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr;
	if(cpu.cr._0.protect_enable == 1) lnaddr = seg_translate(addr, sreg);
		else lnaddr = addr;
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr;
	if(cpu.cr._0.protect_enable == 1) lnaddr = seg_translate(addr, sreg);
		else lnaddr = addr;
	lnaddr_write(lnaddr, len, data);
}

