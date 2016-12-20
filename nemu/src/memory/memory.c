#include "common.h"
#include "cpu/reg.h"

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

uint32_t mmio_read(hwaddr_t, size_t, int);
void mmio_write(hwaddr_t, size_t, uint32_t, int);

uint32_t is_mmio(hwaddr_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int map_NO = is_mmio(addr);
//	if(map_NO == -1) printf("%x\n", addr);
	return map_NO == -1 ? cache_read(addr, len) & (~0u >> ((4 - len) << 3)) :
		mmio_read(addr, len, map_NO) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int32_t map_NO = is_mmio(addr);
	map_NO == -1 ? cache_write(addr, len, data) :
		mmio_write(addr, len, data, map_NO);
}

hwaddr_t page_translate(lnaddr_t, uint32_t);

#define limit 0x1000

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	hwaddr_t hwaddr;
	if(cpu.cr._0.paging == 1) {
//	hwaddr = page_translate(addr, len);

		if((addr & 0xfff) + len <= limit) hwaddr = page_translate(addr, len);
		else {
			uint32_t off = addr & 0xfff;
//			printf("%x %d %x %d %d\n", addr, len, off, limit - off, len - limit + off);
			hwaddr_t hwaddr2;
			hwaddr = page_translate(addr, limit - off);
			hwaddr2 = page_translate(addr + limit - off, len - limit + off);
			return hwaddr_read(hwaddr, limit - off) + 
				(hwaddr_read(hwaddr2, len - limit + off) << ((limit - off) * 8));
		}
	}
		else hwaddr = addr;	
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_t hwaddr;
	if(cpu.cr._0.paging == 1) hwaddr = page_translate(addr, len);
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

