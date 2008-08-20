#include "port.h"

unsigned char inb(unsigned short port){
	unsigned char ret;
	asm volatile ("inb %1,%0":"=a"(ret):"Nd"(port));
	return ret;
}

void outb(unsigned short port, unsigned char data){
	asm volatile("outb %0,%1"::"a"(data), "Nd" (port));
}

unsigned short inw(unsigned short port){
	unsigned short ret;
	asm volatile ("inw %1,%0":"=a"(ret):"Nd"(port));
	return ret;
}
void outw(unsigned short port, unsigned short data){
	asm volatile("outw %0,%1"::"a"(data), "Nd" (port));
}
