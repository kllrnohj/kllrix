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

struct page_directory_entry
{
	unsigned int present : 1;
	unsigned int read_write : 1;
	unsigned int user_supervisor : 1;
	unsigned int write_through : 1;
	unsigned int cache_disabled : 1;
	unsigned int accessed : 1;
	unsigned int zero : 1;
	unsigned int page_size : 1;
	unsigned int global : 1;
	unsigned int available : 3;
	unsigned int page_table_address : 20;
};

struct page_directory
{
	struct page_directory_entry entries[1024];
};

struct page_table_entry
{
	unsigned int present : 1;
	unsigned int read_write : 1;
	unsigned int user_supervisor : 1;
	unsigned int write_through : 1;
	unsigned int cache_disabled : 1;
	unsigned int accessed : 1;
	unsigned int dirty : 1;
	unsigned int zero : 1;
	unsigned int global : 1;
	unsigned int available : 3;
	unsigned int physical_page_address : 20;
};

struct page_table
{
	struct page_table_entry entries[1024];
};

void kVirtualInit();
void* kallocVirtualPages(size_t size);
int kfreeVirtualPages(void* ptr, size_t size);

}

#endif /* KVIRTUALALLOC_H_ */
