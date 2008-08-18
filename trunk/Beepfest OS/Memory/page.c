#include "page.h"
#include "../Video/video.h"

void start_paging(){
	kprint("Entering the page creating function...\n");
	unsigned int *page_directory = (unsigned int *) 0x00200000;
	unsigned int *page_table = (unsigned int *) 0x00200000;
	unsigned int address = 0;
	unsigned int a;
	unsigned int i;
	
	kprint("Beginning page directory and table loops... \n");
	for(a = 0; a < 1024; a++){
		kprint("Setting up page table ");
		kprintUN(a, 10);
		kprint(" page_table address: ");
		kprintUN((unsigned int)page_table, 16);
		kprint("\n");
		kprint("mapping address: ");
		kprintUN(address, 16);
		kprint("\n");
		for(i = 0; i < 1024; i++){
			page_table[i] = address | 3; // 3 == 011 for supervisor, read+write and present
			address += 0x1000;
		}
		
		page_directory[a] = (unsigned int)page_table;
		page_directory[a] = page_directory[a] | 3;
		
		page_table = (unsigned int*)(((unsigned int)page_table) + 0x1000);
	}
	
	kprint("Finished creating page directory, notifying cr3\n");
	
	writecr3((unsigned int)page_directory);
	kprint("cr3 notified. Updating cr0.\n");
	unsigned int temp = readcr0();
	kprint("Read from cr0: ");
	kprintUN(temp, 2);
	kprint(" Beginning cr0 writing...\n");
	writecr0(temp | 0x80000000);
	kprint("cr0 updating, paging enabled.\n");
}
