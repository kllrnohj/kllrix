/*
 * kernel.h
 *
 *  Created on: Oct 2, 2008
 *      Author: kllrnohj
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "include/multiboot.h"
#include "video/VBEDriver.h"
#include "video/FBConsole.h"
#include "include/stdint.h"
#include "include/kstdio.h"

#define inportb inb
#define outportb outb
#define KERNEL_VIRTUAL_BASE 0xC0000000
#define PAGE_TABLE_STORAGE  0xFFC00000

unsigned char inb (unsigned short _port);
void outb (unsigned short _port, unsigned char _data);
unsigned long inl (unsigned short _port);
void outl (unsigned short _port, unsigned long _data);

// globals
extern multiboot_info_t* g_mbinfo;

class Kernel
{
public:
	Kernel(multiboot_info_t *mbt);
protected:
	multiboot_info_t* ShiftToUpper(multiboot_info_t* mbt);
	char* m_commandline;
};

#endif /* KERNEL_H_ */
