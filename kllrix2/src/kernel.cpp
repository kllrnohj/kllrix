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

extern "C"void _main (multiboot_info_t* mbd, unsigned int magic, uintptr_t* bpd);

namespace Globals {
VideoDriver* Video;
VirtualTerminal* Terminal;
}

/* We will use this later on for reading from the I/O ports to get data
 *  from devices such as the keyboard. We are using what is called
 *  'inline assembly' in these routines to actually do the work */
unsigned char inb(unsigned short _port) {
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
 *  will be used in the next tutorial for changing the textmode cursor
 *  position. Again, we use some inline assembly for the stuff that simply
 *  cannot be done in C */
void outb(unsigned short _port, unsigned char _data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

unsigned long inl(unsigned short _port) {
	unsigned long rv;
	__asm__ __volatile__ ("inl %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}
void outl(unsigned short _port, unsigned long _data) {
	__asm__ __volatile__ ("outl %1, %0" : : "dN" (_port), "a" (_data));
}

// Globals
multiboot_info_t *g_mbinfo;
FBConsole* g_con;

multiboot_info_t* Kernel::ShiftToUpper(multiboot_info_t* mbt) {
	for (;;)
		;
	if (!KERNEL_VIRTUAL_BASE)
		return mbt;
	if (!mbt)
		return 0;
	// first shift the mbt address
	mbt += KERNEL_VIRTUAL_BASE;
	// now shift its pointers
	if (mbt->boot_loader_name)
		mbt->boot_loader_name += KERNEL_VIRTUAL_BASE;
	if (mbt->cmdline)
		mbt->cmdline += KERNEL_VIRTUAL_BASE;
	if (mbt->mmap_addr)
		mbt->mmap_addr += KERNEL_VIRTUAL_BASE;
	if (mbt->mods_addr)
		mbt->mods_addr += KERNEL_VIRTUAL_BASE;
	if (mbt->vbe_control_info)
		mbt->vbe_control_info += KERNEL_VIRTUAL_BASE;
	if (mbt->vbe_mode_info)
		mbt->vbe_mode_info += KERNEL_VIRTUAL_BASE;
	return mbt;
}

Kernel::Kernel(multiboot_info_t* mbt) {
	for (;;)
		; // we shouldn't get here yet, but just in case...
	mbt = ShiftToUpper(mbt);

	// before copying the mbt info, we need some things first
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	Memory::Init(mbt);

}

#define X86_CR4_VME		0x0001	/* enable vm86 extensions */
#define X86_CR4_PVI		0x0002	/* virtual interrupts flag enable */
#define X86_CR4_TSD		0x0004	/* disable time stamp at ipl 3 */
#define X86_CR4_DE		0x0008	/* enable debugging extensions */
#define X86_CR4_PSE		0x0010	/* enable page size extensions */
#define X86_CR4_PAE		0x0020	/* enable physical address extensions */
#define X86_CR4_MCE		0x0040	/* Machine check enable */
#define X86_CR4_PGE		0x0080	/* enable global pages */
#define X86_CR4_PCE		0x0100	/* enable performance counters at ipl 3 */
#define X86_CR4_OSFXSR		0x0200	/* fast FPU save/restore */
#define X86_CR4_OSXMMEXCPT	0x0400	/* KNI (MMX2) unmasked exception 16 */
/* handler is available */

/*
 * Save the cr4 feature set we're using (ie
 * Pentium 4MB enable and PPro Global page
 * enable), so that any CPU's that boot up
 * after us can get the correct flags.
 */
unsigned long mmu_cr4_features = 0;

static inline void set_in_cr4(unsigned long mask) {
	mmu_cr4_features |= mask;
	__asm__("movl %%cr4,%%eax\n\t"
			"orl %0,%%eax\n\t"
			"movl %%eax,%%cr4\n"
			: : "irg" (mask)
			:"ax");
}

enum cpuid_requests {
	CPUID_GETVENDORSTRING,
	CPUID_GETFEATURES,
	CPUID_GETTLB,
	CPUID_GETSERIAL,

	CPUID_INTELEXTENDED = 0x80000000,
	CPUID_INTELFEATURES,
	CPUID_INTELBRANDSTRING,
	CPUID_INTELBRANDSTRINGMORE,
	CPUID_INTELBRANDSTRINGEND,
};

/** issue a single request to CPUID. Fits 'intel features', for instance
 *  note that even if only "eax" and "edx" are of interrest, other registers
 *  will be modified by the operation, so we need to tell the compiler about it.
 */
static inline void cpuid(int code, int *a, int *d) {
	asm volatile("cpuid":"=a"(*a),"=d"(*d):"0"(code):"ecx","ebx");
}

static void inline read_msr(uint32_t msr, uint32_t *a, uint32_t* d) {
	asm volatile ("rdmsr": "=a" (*a), "=d" (*d): "c" (msr) : );
}

static void inline write_msr(uint32_t msr, uint32_t a, uint32_t d) {
	asm volatile ("wrmsr": : "a" (a), "d" (d), "c" (msr) : );
}

/** issue a complete request, storing general registers output as a string
 */
static inline int cpuid_string(int code, int where[4]) {
	int highest;
	asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
			"=c"(*(where+2)),"=d"(*(where+3)):"0"(code));
	return highest;
}

void _main(multiboot_info_t* mbt, unsigned int, uintptr_t* bpd) {
	set_in_cr4(X86_CR4_OSFXSR| X86_CR4_OSXMMEXCPT | X86_CR4_PSE);
	__asm__ __volatile__ ("fninit");
	uint32_t eax = 0, edx = 0;
	read_msr(0x277, &eax, &edx);
	write_msr(0x277, eax, (edx & ~0xFF) | 0x01);
	Memory::BootPageDirectory = bpd;
	// shift mbt to our virtual mapped area
	g_mbinfo = (multiboot_info_t*) ((uintptr_t) mbt + KERNEL_VIRTUAL_BASE);
	g_mbinfo->vbe_mode_info = g_mbinfo->vbe_mode_info + KERNEL_VIRTUAL_BASE;
	g_mbinfo->vbe_control_info = g_mbinfo->vbe_control_info
			+ KERNEL_VIRTUAL_BASE;
	// Identity map the VBE physical address
	struct vbe_mode* mode = (struct vbe_mode*) g_mbinfo->vbe_mode_info;
	struct vbe_controller* controller =
			(struct vbe_controller*) g_mbinfo->vbe_control_info;
	uintptr_t page_nr = mode->phys_base >> 22;
	Memory::BootPageDirectory[page_nr] = (mode->phys_base & 0xFFC00000)
			| 0x1083;
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
	//printf("inl: %d\n", inl(0x01CF) / 1024 / 1024);
	//printf("Flags: %x\nBoot Device: %x\n", g_mbinfo->flags, g_mbinfo->boot_device);
	//printf("Command Line: %s\n", g_mbinfo->cmdline + KERNEL_VIRTUAL_BASE);
	//printf("Num drives: %d\n", g_mbinfo->drives_length);
	//printf("Globals::Video: %#8x\nGlobals::Terminal: %#8x\n", Globals::Video, Globals::Terminal);
	printf("VRam: %dMB\n", (controller->total_memory * 64) / 1024);
	printf("Version: %#4x\n", controller->version);
	printf("VBE_PMI: %#8x\n", g_mbinfo->vbe_interface_seg);

	read_msr(0x277, &eax, &edx);
	printf("MSR 227H: %#8x %#8x\n", edx, eax);
	printf("PageDirectory: %#8x\n", Memory::PageDirectory->entries[KERNEL_VIRTUAL_BASE >> 22]);

	// lets probe the PCI bus :)
	//PCIDriverManager *pci = new PCIDriverManager();
	//pci->Probe();

	for (;;) {
		__asm__ __volatile__ ("hlt");
		// "sleep" :)
	}
}
