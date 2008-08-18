#ifndef __KERNEL_H__
#define __KERNEL_H__

#define NULL 0

void setToDo(void*);
void setToDoCP(char*);

struct regs{
	unsigned int gs, fs, es, ds; // pushed segs last
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by 'pusha'
	unsigned int int_no, err_code; // from push byte # and etc
	unsigned int eip, cs, eflags, useresp, ss; // pushed by CPU automagically
};

#endif

