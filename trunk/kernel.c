#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "isrs.h"
#include "irq.h"
#include "timer.h"
#include "kernel.h"
#include "kbd.h"
#include "multiboot.h"
//#include "rand.h"

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

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define HEAD_C '#'
#define BODY_C '*'
#define FRUIT_C '@'
#define BACK 0x0
#define FORE 0xF

void gameLoop()
{
	kcls();
	unsigned short attrib = (BACK << 4) | ((FORE & 0x0F) << 8);
	unsigned short *vgaBuffer = (unsigned short*) 0xB8000;
	unsigned int playerPositions[256];
	unsigned int playerCount = 1;
	playerPositions[0] = (5 * 80) + 30;
	unsigned int direction = RIGHT;
	unsigned int last_tick;
	for(;;) 
	{
		last_tick = timer_ticks;
		// check keypresses
		switch (keyDown) {
		case ARROW_LEFT: direction = LEFT; break;
		case ARROW_RIGHT: direction = RIGHT; break;
		case ARROW_UP: direction = UP; break;
		case ARROW_DOWN: direction = DOWN; break;
		}
		
		int clearPlayerPosition = playerPositions[playerCount - 1];
		// shift positions
		if (playerCount > 1) {
			int i = 0;
			for (i = playerCount - 2; i >= 0; i--)
				playerPositions[i+1] = playerPositions[i];
		}
		
		int tr = playerPositions[0] / 80;
		int tc = playerPositions[0] % 80;
		
		// move player
		switch (direction) {
		case LEFT:
			tc--;
			break;
		case RIGHT:
			tc++;
			break;
		case UP:
			tr--;
			break;
		case DOWN:
			tr++;
			break;
		}
		
		// edge check
		if (tc >= 80)
			tc = 0;
		if (tc < 0)
			tc = 79;
		if (tr < 0)
			tr = 24;
		if (tr >= 25)
			tr = 0;
		
		// update for edge check
		playerPositions[0] = (tr * 80) + tc;
		
		// draw
		// clear old position
		vgaBuffer[clearPlayerPosition] = attrib;
		// draw new position
		vgaBuffer[playerPositions[0]] = attrib | (HEAD_C & 0xFF);
		
		if (playerCount > 1) {
			// redraw previous position
			vgaBuffer[playerPositions[1]] = attrib | (BODY_C & 0xFF);
		}
		
		// 18 "fps"
		while (last_tick == timer_ticks);
	}
}

void seedRandomUsingRTC()
{
	unsigned long seed = 0;
	//init_genrand(seed);
}

unsigned char readCMOS(unsigned char addr)
{
   unsigned char r;
   outb(0x70,addr);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
   r = inb(0x71);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
   return r;
}

#define BCD2BIN(bcd) ((((bcd)&15) + ((bcd)>>4)*10))
void get_time_stuff()
{
	asm("cli");
	kputi(BCD2BIN(readCMOS(0x0)));
	kputc('\n');
	kputi(BCD2BIN(readCMOS(0x2)));
	kputc('\n');
	kputi(BCD2BIN(readCMOS(0x4)));
	kputc('\n');
	kputi(BCD2BIN(readCMOS(0x7)));
	kputc('\n');
	
	/*
	now.sec = BCD2BIN(readCMOS(0x0));
	now.min = BCD2BIN(readCMOS(0x2));
	now.hour = BCD2BIN(readCMOS(0x4));
	now.day = BCD2BIN(readCMOS(0x7));
	now.month = BCD2BIN(readCMOS(0x8));
	now.year = BCD2BIN(readCMOS(0x9));
	*/
	asm("sti");
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
	con.buffer[81] = (0x0F << 8) | ('A' & 0xFF);
	for(;;);
	get_time_stuff();
	seedRandomUsingRTC();
	kputc('\n');
	//kputi(genrand_int32());
	kputc('\n');
	//kputi(genrand_int32());
	kputc('\n');
	//kputi(genrand_int32());
	for(;;);
	timer_install();
	//timer_phase(2); // this doesn't work right in virtualbox, so leave at default 18.222hz
	keyboard_install();
	gameLoop(); // doesn't return
	for(;;);
}
