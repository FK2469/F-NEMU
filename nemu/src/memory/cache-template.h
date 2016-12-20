#include "common.h"
#include "burst.h"
#include "rand.h"
#include "misc.h"

#include "memory/cache-start.h"

cache_t cache;

uint32_t rand(int);

#if LEVEL == MAX_LEVEL
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
#else 
uint32_t concat3(cache_read_, LEVEL, I)(hwaddr_t, size_t);
void concat3(cache_write_, LEVEL, I)(hwaddr_t, size_t, uint32_t);
void concat3(init_cache_, LEVEL, I)();
#endif

void concat(init_cache_, LEVEL)() {
	memset(cache.set, 0, sizeof cache.set);
#if LEVEL != MAX_LEVEL
	concat3(init_cache_, LEVEL, I)();
#endif
}

#ifdef WRITE_BACK
void write_back(uint32_t set, uint32_t line, hwaddr_t addr) {
	if(!cache.set[set].dirty[line]) return;
	uint32_t col;
	for(col = 0; col < NR_COL; ++ col) {
#if LEVEL == MAX_LEVEL
		dram_write((addr & ~COL_MASK) + col, 1, cache.set[set].data[line][col]);
#else 
		concat3(cache_write_, LEVEL, I)((addr & ~COL_MASK) + col, 1, cache.set[set].data[line][col]);
#endif
	}

}
#endif

void write_cache(uint32_t set, uint32_t line, uint32_t flag, uint32_t addr) {
	cache.set[set].valid[line] = true;
	cache.set[set].dirty[line] = false;
	cache.set[set].flag[line] = flag;
	uint32_t col;
	for(col = 0; col < NR_COL; ++ col) {
#if LEVEL == MAX_LEVEL
		cache.set[set].data[line][col] = dram_read((addr & ~COL_MASK) + col, 1);
#else
		cache.set[set].data[line][col] = concat3(cache_read_, LEVEL, I)((addr & ~COL_MASK) + col, 1);
#endif
	}
}

void cache_set_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	uint32_t line, line_ = 0;
	bool full = true, find = false;
	for(line = 0; line < NR_LINE; ++ line) {
		if(cache.set[set].valid[line]) {
			if(cache.set[set].flag[line] == flag) {
				memcpy(data, cache.set[set].data[line] + col, BURST_LEN);
				find = true;
				break;
			}
		} else {
			full = false;
			line_ = line;
		}
	}
	if(!find) {
		if(full) {
			line_ = rand(addr) & LINE_MASK;
#ifdef WRITE_BACK
			write_back(set, line_, addr);
#endif
		}
		write_cache(set, line_, flag, addr);
		memcpy(data, cache.set[set].data[line_] + col, BURST_LEN);
	}
}

void cache_set_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	uint32_t line;
#ifdef WRITE_ALLOCATE	
	bool full = true, find = false;
	uint32_t line_ = 0;
#endif
	
	for(line = 0; line < NR_LINE; ++ line) {
		if(cache.set[set].valid[line]) {
			if(cache.set[set].flag[line] == flag) {
#ifdef WRITE_ALLOCATE	
				find = true;
#endif
				cache.set[set].dirty[line] = true;
				memcpy_with_mask(cache.set[set].data[line] + col, data, BURST_LEN, mask);
				break;
			}
		} else {
#ifdef WRITE_ALLOCATE	
			full = false;
			line_ = line;
#endif
		}
	}

#ifdef WRITE_ALLOCATE	
	if(!find) {
		if(full) {
			line_ = rand(addr) & LINE_MASK;
#ifdef WRITE_BACK
			write_back(set, line_, addr);
#endif
		}
		write_cache(set, line_, flag, addr);
		memcpy_with_mask(cache.set[set].data[line_] + col, data, BURST_LEN, mask);
	}
#endif

}

uint32_t cache_read(hwaddr_t addr, size_t len) {

	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	cache_set_read(addr, temp);

	if(offset + len > BURST_LEN) {
		cache_set_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	
	return unalign_rw(temp + offset, 4);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	cache_set_write(addr, temp, mask);

	if(offset + len > BURST_LEN) {
		cache_set_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
#ifdef WRITE_THOUGH
	#if LEVEL == MAX_LEVEL
		dram_write(addr, len, data);
	#else
		concat3(cache_write_, LEVEL, I)(addr, len, data);
	#endif
#endif
}

#include "memory/cache-end.h"

