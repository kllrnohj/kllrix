/*
 * KVirtualAlloc.h
 *
 *  Created on: Oct 4, 2008
 *      Author: kllrnohj
 */

#ifndef KVIRTUALALLOC_H_
#define KVIRTUALALLOC_H_

#include "liballoc.h"
#include "StackAllocator.h"
#include "kernel.h"

namespace Memory {

void kVirtualInit();
void* kallocVirtualPages(size_t size);
int kfreeVirtualPages(void* ptr, size_t size);

}

#endif /* KVIRTUALALLOC_H_ */
