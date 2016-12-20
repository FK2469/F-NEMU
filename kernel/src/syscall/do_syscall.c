#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
void serial_printc(char);

int fs_open(const char *pathname, int flags);
int fs_read(int fd, void *buf, int len);
int fs_write(int fd, void *buf, int len);
int fs_lseek(int fd, int offset, int whence);
int fs_close(int fd);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

static void sys_write(TrapFrame *tf) {
#ifdef HAS_DEVICE
	int i;
	for(i = 0; i < tf->edx; ++ i)
		serial_printc(*(char *)(tf->ecx + i));
#else				
	asm volatile (".byte 0xd6" : : "a"(2), "c"(tf->ecx), "d"(tf->edx));
#endif
	tf->eax = tf->edx;
}

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk: /*panic("@@@");*/sys_brk(tf); break;
		case SYS_write: sys_write(tf); break;
		case SYS_open : 
			tf->eax = fs_open((char *)tf->ebx, tf->ecx); break;
		case SYS_read : 
//			Log("%x %x %x %x %x\n", tf->ebx, tf->ecx, tf->edx, tf->edi, tf->esi);
			tf->eax = fs_read(tf->ebx, (void *)tf->ecx, tf->edx); break;
		case SYS_lseek : tf->eax = fs_lseek(tf->ebx, tf->ecx, tf->edx); break;
		case SYS_close : tf->eax = fs_close(tf->ebx); break;

		/* TODO: Add more system calls. */

		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

