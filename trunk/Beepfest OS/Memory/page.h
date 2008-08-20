#ifndef __PAGE_H__
#define __PAGE_H__

#include "../Boot/multiboot.h"

#define PAGE_ALLOCATION_BYTE 9
#define PAGE_RESERVED_BYTE 10

extern unsigned int readcr0();
extern void writecr0(unsigned int value);
extern unsigned int readcr3();
extern void writecr3(unsigned int value);

void start_paging(multiboot_info_t* b);

void* getPageDirectory();
void* getVirtualPageDirectory();

#endif

