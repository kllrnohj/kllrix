#include "page.h"
#include "../Video/video.h"

void* pDirectory = (void*)0x00200000;
void* pVirtualDirectoryPointer;
char* pVGAVirtualPointer = (void*)0xA0000;
multiboot_info_t* mbd;

void* getVirtualPageDirectory(){
	return pVirtualDirectoryPointer;
}

unsigned char inReservedPage(void* address){
	// include kernel in the shizit
	if(address >= 0x00100000 && address < 0x00200000)
		return 1; // it is in the megabyte (i guess...) I give the kernel code
	if(mbd->flags & (1<<6)){
		struct memory_map* mmap = mbd->mmap_addr;
		while(mmap < mbd->mmap_addr + mbd->mmap_length){
			if(address >= mmap->base_addr_low && address <= (mmap->base_addr_low + mmap->length_low)){
				// the address is in a section, see if it is reserved
				if(mmap->type != 1){
					// it is reserved section
					return 1;
				}
			}
			mmap = (struct memory_map*)((unsigned int)mmap + mmap->size + sizeof(unsigned int));
		}
	}
	return 0;
}

void* getPageDirectory(){
	return pDirectory;
}

void start_paging(multiboot_info_t* b){
	mbd = b;
	kprint("Entering the page creating function...\n");
	pDirectory = (void*)0x00200000;
	unsigned int *page_directory = (unsigned int *) pDirectory;
	// gonna start the tables RIGHT after directory
	unsigned int *page_table = (unsigned int *) ((int)pDirectory + 0x00001000);
	unsigned int address = 0;
	unsigned int a;
	unsigned int i;
	
	kprint("Beginning page directory and table loops... \n");
	for(a = 0; a < 1024; a++){
		/*kprint("Setting up page table ");
		kprintUN(a, 10);
		kprint(" page_table address: ");
		kprintUN((unsigned int)page_table, 16);
		kprint("\n");
		kprint("mapping address: ");
		kprintUN(address, 16);
		kprint("\n");*//* DEBUGGING INFO */
		for(i = 0; i < 1024; i++){
			// bits 11...9 are mine Mine MINE!!!! so... bit 9 now indicates if it is allocated
			// 0 means it's free, 1 means allocated. So that actually changes nothing at this
			// point, but good to keep in mind.
			// bit 10 (PAGE_RESERVED_BYTE) signifies that it is a reserved section, and cannot be accessed at all
			if(address == 0x00200000){
				page_table[i] = (address | 3) | (1 << PAGE_RESERVED_BYTE);
				// need to set virtual pointer....
				// virtual pointer is 1024^2 * a 
				// (0x1000 * 1024) * a + (0x1000 * i)
				// 400000 = (0x1000 * 1024)
				pVirtualDirectoryPointer = ((0x400000 * a) + (0x1000 * i));
				kprint("Virtual directory pointer: ");
				kprintUN((unsigned int)pVirtualDirectoryPointer, 16);
				kprint("\n");
			}else if(address == 0xA0000){
				pVGAVirtualPointer = ((0x400000 * a) + (0x1000 * i));
			}
			if(inReservedPage((void*)address)){
				//kprint("Found ");
				//kprintUN(address, 16);
				//kprint(" as reserved... marking\n");
				page_table[i] = (address | 3) | (1 << PAGE_RESERVED_BYTE);
			}else{
				page_table[i] = address | 3; // 3 == 011 for supervisor, read+write and present
			}
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
