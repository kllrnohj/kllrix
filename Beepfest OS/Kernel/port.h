#ifndef __PORT_H__
#define __PORT_H__

#define in(a) inb(a)
#define out(a, b) outb(a, b)

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);

unsigned short inw(unsigned short port);
void outw(unsigned short port, unsigned short data);

#endif

