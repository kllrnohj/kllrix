/*
 * kernel.cpp
 *
 *  Created on: Oct 2, 2008
 *      Author: kllrnohj
 */

#include "kernel.h"
#include "i386/gdt.h"
#include "i386/idt.h"
#include "i386/irq.h"
#include "i386/isrs.h"
#include "i386/timer.h" // temporary. Will use APIC later
#include "i386/kbd.h"
#include "memory/StackAllocator.h"
#include "memory/kmem.h"
#include "drivers/bus/PCIDriverManager.h"
#include "include/kstdio.h"

extern "C" void _main (multiboot_info_t* mbd, unsigned int magic, uintptr_t* bpd);

namespace Globals
{
	VideoDriver* Video;
	VirtualTerminal* Terminal;
}

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

unsigned long inl (unsigned short _port)
{
    unsigned long rv;
    __asm__ __volatile__ ("inl %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}
void outl (unsigned short _port, unsigned long _data)
{
    __asm__ __volatile__ ("outl %1, %0" : : "dN" (_port), "a" (_data));
}

// Globals
multiboot_info_t *g_mbinfo;
FBConsole* g_con;

multiboot_info_t* Kernel::ShiftToUpper(multiboot_info_t* mbt)
{
	for(;;);
	if (!KERNEL_VIRTUAL_BASE) return mbt;
	if (!mbt) return 0;
	// first shift the mbt address
	mbt += KERNEL_VIRTUAL_BASE;
	// now shift its pointers
	if (mbt->boot_loader_name) mbt->boot_loader_name += KERNEL_VIRTUAL_BASE;
	if (mbt->cmdline) mbt->cmdline += KERNEL_VIRTUAL_BASE;
	if (mbt->mmap_addr) mbt->mmap_addr += KERNEL_VIRTUAL_BASE;
	if (mbt->mods_addr) mbt->mods_addr += KERNEL_VIRTUAL_BASE;
	if (mbt->vbe_control_info) mbt->vbe_control_info += KERNEL_VIRTUAL_BASE;
	if (mbt->vbe_mode_info) mbt->vbe_mode_info += KERNEL_VIRTUAL_BASE;
	return mbt;
}

Kernel::Kernel(multiboot_info_t* mbt)
{
	for(;;); // we shouldn't get here yet, but just in case...
	mbt = ShiftToUpper(mbt);

	// before copying the mbt info, we need some things first
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	Memory::Init(mbt);

}

void _main (multiboot_info_t* mbt, unsigned int magic, uintptr_t* bpd)
{
	Memory::BootPageDirectory = bpd;
	// shift mbt to our virtual mapped area
	g_mbinfo = (multiboot_info_t*)((uintptr_t)mbt + KERNEL_VIRTUAL_BASE);
	g_mbinfo->vbe_mode_info = g_mbinfo->vbe_mode_info + KERNEL_VIRTUAL_BASE;
	g_mbinfo->vbe_control_info = g_mbinfo->vbe_control_info + KERNEL_VIRTUAL_BASE;
	// Identity map the VBE physical address
	struct vbe_mode* mode = (struct vbe_mode*)g_mbinfo->vbe_mode_info;
	uintptr_t page_nr = mode->phys_base >> 22;
	Memory::BootPageDirectory[page_nr] = (mode->phys_base & 0xFFFFFF00) | 0x83;
	// shift memory structure information to virtual area
	g_mbinfo->mmap_addr += KERNEL_VIRTUAL_BASE;

	Memory::Init(g_mbinfo);

	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");

	timer_install();
	keyboard_install();

	VideoDriver* video = VideoDriver::GetDriver(g_mbinfo);
	VirtualTerminal *vt = new VirtualTerminal(video, 80, 25);
	Globals::Video = video;
	Globals::Terminal = vt;

	printf("Hello, World!\nIsn't today %s\n", "great?");

	printf("Successfully booted :)\n");
	printf("inl: %d\n", inl(0x01CF) / 1024 / 1024);
	printf("Flags: %x\nBoot Device: %x\n", g_mbinfo->flags, g_mbinfo->boot_device);
	printf("Command Line: %s\n", g_mbinfo->cmdline + KERNEL_VIRTUAL_BASE);
	printf("Num drives: %d\n", g_mbinfo->drives_length);
	printf("Globals::Video: %#8x\nGlobals::Terminal: %#8x\n", Globals::Video, Globals::Terminal);

	// lets probe the PCI bus :)
	PCIDriverManager *pci = new PCIDriverManager();
	pci->Probe();

	for(;;); // never allow this to return. NEVER!
}
