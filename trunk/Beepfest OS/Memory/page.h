#ifndef __PAGE_H__
#define __PAGE_H__

extern unsigned int readcr0();
extern void writecr0(unsigned int value);
extern unsigned int readcr3();
extern void writecr3(unsigned int value);

void start_paging();

#endif

