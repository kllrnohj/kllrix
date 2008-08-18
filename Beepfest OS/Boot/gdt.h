#ifndef __GDT_H__
#define __GDT_H__

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();

#endif

