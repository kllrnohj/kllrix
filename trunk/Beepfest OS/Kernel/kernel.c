#include "kernel.h"
#include "../Video/video.h"
#include "port.h"
#include "../Console/SpaceInvaders.h"
#include "../Boot/gdt.h"
#include "../Boot/irq.h"
#include "timer.h"
#include "../Keyboard/kb.h"
#include "../Memory/page.h"
#include "rand.h"
#include "../Boot/multiboot.h"

static int keepGoing = 1;

void* todo;
char* todoCP;

void kmain(multiboot_info_t* mbd, unsigned int magic);
void sleep(unsigned int milliseconds);

void kmain(multiboot_info_t* mbd, unsigned int magic){
	// kernel entry point.
	//unsigned char* videoram = (unsigned char*) 0xb8000;
	//videoram[0] = 65;
	//videoram[1] = 0x07;
	//setMode(0x13);
	setMode(320, 200, 8); // HOLY SHIT THIS WORKS
	clrscr();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	start_paging(mbd);
	keyboard_install();
	init_genrand(get_timer_ticks());
	__asm__ __volatile__ ("sti");
	kprint("Hello, We have booted successfully!\n\tTip: type help for command info\n");
		
	while(1){
		if(todo != NULL){
			void(*function)() = todo;
			function(todoCP);
			todo = NULL;
		}
	}
	
}

void sleep(unsigned int milliseconds){
	while(milliseconds--){
		int x;
		for(x = 0; x < 23330; x++){
			__asm__ ("nop");
		}
	}
}

void setToDo(void* value){
	todo = value;
}
void setToDoCP(char* value){
	todoCP = value;
}
