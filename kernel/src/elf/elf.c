#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>
//#include <stdio.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define HEAD_SIZE 4096
#define STACK_SIZE (1 << 20)
#define _SIZE_ (1 << 20)
//0xa0000 - 1

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[HEAD_SIZE];
	uint8_t buf_[_SIZE_];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, HEAD_SIZE);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, HEAD_SIZE);
#endif
	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	int cnt;
	/* Load each program segment */
	for(cnt = 0; cnt < elf->e_phnum; ++ cnt) {
		/* Scan the program header table, load each segment into memory */
/*
#ifdef HAS_DEVICE
		ide_read(buf_, ELF_OFFSET_IN_DISK + elf->e_ehsize + cnt * elf->e_phentsize, elf->e_phentsize);
#else
		ramdisk_read(buf_, ELF_OFFSET_IN_DISK + elf->e_ehsize + cnt * elf->e_phentsize, elf->e_phentsize);
#endif
*/
		ph = (void*)(buf + elf->e_ehsize + cnt * elf->e_phentsize);
//		ph = (void*)buf_;
		if(ph->p_type == PT_LOAD) {
			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
#ifdef HAS_DEVICE
		ide_read(buf_, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
#else
		ramdisk_read(buf_, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
#endif		
			
			uint32_t hwaddr = 
				mm_malloc(ph->p_vaddr, ph->p_memsz);

			memcpy((void *)hwaddr, (void *)(buf_), ph->p_filesz);
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
			
//			Log("%x %x %x %x", ph->p_vaddr, ph->p_filesz, ph->p_memsz, ph->p_vaddr + ph->p_filesz);
			memset((void *)hwaddr + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);

#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif

//	HIT_GOOD_TRAP;
	return entry;
}
