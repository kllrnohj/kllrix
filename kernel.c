#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "isrs.h"
#include "irq.h"
#include "timer.h"
#include "kernel.h"
#include "kbd.h"
#include "multiboot.h"

/* We will use this later on for reading from the I/O ports to get data
*  from devices such as the keyboard. We are using what is called
*  'inline assembly' in these routines to actually do the work */
unsigned char inb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
    
/** issue a single request to CPUID. Fits 'intel features', for instance
 *  note that even if only "eax" and "edx" are of interrest, other registers
 *  will be modified by the operation, so we need to tell the compiler about it.
 */
static inline void cpuid(int code, dword *a, dword *d) {
  asm volatile("cpuid":"=a"(*a),"=d"(*d):"0"(code):"ecx","ebx");
}
    
/** issue a complete request, storing general registers output as a string
 */
static inline int cpuid_string(int code, dword where[4]) {
  int highest;
  asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
               "=c"(*(where+2)),"=d"(*(where+3)):"0"(code));
  return highest;
}

void kmain( multiboot_info_t* mbd, unsigned int magic )
{
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	console_s con;
	con.buffer = (unsigned short *)0xB8000;
	con.c_back = 0x0;
	con.c_fore = 0xF;
	con.c_col = 0;
	con.c_row = 0;
	cur_c = &con;
	kcls();
	kputs("Hello, World!\n");
	
	
	
	timer_install();
	timer_phase(100);
	keyboard_install();
	for(;;);
}
