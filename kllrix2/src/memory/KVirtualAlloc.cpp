/*
 * KVirtualAlloc.cpp
 *
 *  Created on: Oct 4, 2008
 *      Author: kllrnohj
 */

#include "KVirtualAlloc.h"

int liballoc_lock()
{
	// for the moment, just disable interrupts.
	__asm__ __volatile__ ("cli");
	return 0;
}

int liballoc_unlock()
{
	// re-enable interrupts
	__asm__ __volatile__ ("sti");
	return 0;
}

// C wrapper for C++ function call
void* liballoc_alloc(size_t size)
{
	return Memory::kallocVirtualPages(size);
}

// C wrapper for C++ function call
int liballoc_free(void* ptr, size_t size)
{
	return Memory::kfreeVirtualPages(ptr, size);
}

namespace Memory {

// page bit map of the kernel's initial 4MB using 64kb pages
static uint64_t kp_bitmap;

void kVirtualInit()
{
	int pages_used = (stack_end - KERNEL_VIRTUAL_BASE) / 65536;
	if ((stack_end - KERNEL_VIRTUAL_BASE) % 65536)
		pages_used++;
	for (int i = 0; i < 64; i++)
	{
		kp_bitmap <<= 1;
		if (pages_used)
		{
			kp_bitmap++;
			pages_used--;
		}
	}

}

void* kallocVirtualPages(size_t size)
{
	// does kp_bitmap have any free space?
	if (((uint32_t)(kp_bitmap >> 32)) != 0xFFFFFFFF ||
			((uint32_t)(kp_bitmap & 0xFFFFFFFF)) != 0xFFFFFFFF)
	{
		int chunk_size = size / 16;
		if (size % 16)
			chunk_size++;
		// Yes, search for a place large enough
		for (int i = 1; i <= 64; i++)
		{
			int s = i;
			for (; !((kp_bitmap >> (64-s)) & 1) && s <= 64; s++);
			if ((s - i) >= chunk_size)
			{
				// set bits
				for (int a = i; a < (i + chunk_size); a++)
					kp_bitmap |= ((uint64_t)1) << (64 - a);
				return (void*) (KERNEL_VIRTUAL_BASE + 0x10000 * (i - 1));
			}
		}
	}

	// failed to find space in the bitmap, for now just fail.
	return 0;
}

int kfreeVirtualPages(void* ptr, size_t size)
{
	if ((uintptr_t)ptr < KERNEL_VIRTUAL_BASE + 0x400000) {
		// round to 64kb if it isn't already
		// this is OK since size will always be the same as one that was allocated
		int chunk_size = size / 16;
		if (size % 16)
			chunk_size++;
		int bstart = ((uintptr_t)ptr - KERNEL_VIRTUAL_BASE) / 0x10000;
		for (int i = bstart; i < (bstart + chunk_size); i++)
		{
			kp_bitmap -= ((uint64_t)1) << (63 - i);
		}
		return 0;
	}

	// failed
	return -1;
}

}
