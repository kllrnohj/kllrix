/*
 * StackAllocator.cpp
 *
 *  Created on: Oct 3, 2008
 *      Author: kllrnohj
 */

#include "StackAllocator.h"
extern uintptr_t knl_end;

namespace Memory {

// "private"

static volatile uintptr_t* pmem_stack;
static volatile uintptr_t pmem_stack_top;
uintptr_t stack_end;

// ---

void Init(multiboot_info_t* mbt)
{
	// align to 4kb
	pmem_stack = (uintptr_t*)(((uintptr_t)&knl_end | 0xFFF)+1);
	pmem_stack_top = 0;

	// First calculate how much stack space is needed
	int maxpages = 0;
	memory_map_t* mmap = (memory_map_t*)mbt->mmap_addr;
	while((unsigned long)mmap < (unsigned long)mbt->mmap_addr + mbt->mmap_length) {
		if ((uintptr_t)mmap->base_addr_low >= 0x100000)
		{
			maxpages += (mmap->length_low / PAGE_SIZE);
		}
		mmap = (memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}

	// Determine the end of the stack and align it to 4kb (last used page)
	stack_end = ((uintptr_t)(pmem_stack + maxpages) | 0xFFF)+1;

	// now loop through again, this time adding pages to the stack
	mmap = (memory_map_t*)mbt->mmap_addr;
	while((unsigned long)mmap < (unsigned long)mbt->mmap_addr + mbt->mmap_length) {
		if ((uintptr_t)mmap->base_addr_low >= 0x100000)
		{
			for (uintptr_t p = mmap->base_addr_low; p < (mmap->base_addr_low + mmap->length_low); p += 4096)
			{
				if (p >= 0x400000) // entire first page_table is spoken for by the kernel
					FreePhysPage(p); // "delete" it to add it to the stack
			}
		}
		mmap = (memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}
	kVirtualInit();
}

void FreePhysPage(uintptr_t addr)
{
	pmem_stack[pmem_stack_top++] = addr;
}

uintptr_t AllocPhysPage()
{
	if (!pmem_stack_top)
	{
		printf("Oh shit, we be outa RAM!\n");
		return 0;
	}
	return pmem_stack[--pmem_stack_top];
}

}
