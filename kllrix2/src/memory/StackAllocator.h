/*
 * StackAllocator.h
 *
 *  Created on: Oct 3, 2008
 *      Author: kllrnohj
 */

#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include "kernel.h"
#include "KVirtualAlloc.h"

namespace Memory {

#define PAGE_SIZE 4096 // 4kb pages
extern uintptr_t *BootPageDirectory;
extern uintptr_t stack_end; // let the KVirtualAlloc know from where it can start handing out "pages" of the initial 4MB reserve
extern struct page_directory *PageDirectory;

// "public"
void Init(multiboot_info_t* mbt);
void FreePhysPage(uintptr_t addr);
uintptr_t AllocPhysPage();

}

#endif /* STACKALLOCATOR_H_ */
