#include "console.h"
#include "kmem.h"
#include "kernel.h"

console_s* cur_c;

void update_cursor(int row, int col)
{
   unsigned short position=(row*80) + col;

   // cursor LOW port to vga INDEX register
   outb(0x3D4, 0x0F);
   outb(0x3D5, (unsigned char)(position&0xFF));
   // cursor HIGH port to vga INDEX register
   outb(0x3D4, 0x0E);
   outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}

void scroll()
{
	int i;
	for (i = 1; i < CON_ROWS; i++)
	{
		kmemcpy(cur_c->buffer + ((i - 1) * CON_COLUMNS),
				cur_c->buffer + (i * CON_COLUMNS),
				CON_COLUMNS);
	}
	kmemset(cur_c->buffer + ((CON_ROWS - 1) * CON_COLUMNS), 0, CON_COLUMNS);
}

void kcls()
{
	int ir, ic;
	for (ir = 0; ir < CON_ROWS; ir++)
		for (ic = 0; ic < CON_COLUMNS; ic++)
			kputc(' ');
	cur_c->c_col = 0;
	cur_c->c_row = 0;
	update_cursor(0,0);
}

void kputi(int i)
{
	if (i < 10) {
		kputc('0' + i);
		return;
	}
	kputi(i / 10);
	kputc('0' + (i % 10));
}

void kputc(char c)
{
	short val;
	switch(c) {
	case '\0': // umm, what? we can't print this!
		return;
	case '\n':
		cur_c->c_col = 0;
		cur_c->c_row++;
		break;
	case '\t':
		kputs("    ");
		return;
	case '\r':
		cur_c->c_row = 0;
		break;
	case '\b':
		if (cur_c->c_col == 0) {
			if (cur_c->c_row == 0) return;
			cur_c->c_row--;
			cur_c->c_col = CON_COLUMNS - 1;
		} else {
			cur_c->c_col--;
		}
	default:
		val  = (cur_c->c_back << 4);
		val |= (cur_c->c_fore & 0x0F) << 8;
		if (c == '\b')
			val |= ' ' & 0xFF;
		else
			val |= c & 0xFF;
		cur_c->buffer[(cur_c->c_row * CON_COLUMNS) + cur_c->c_col] = val;
		if (c == '\b') break;
		cur_c->c_col++;
	}
	if (cur_c->c_col >= CON_COLUMNS)
	{
		cur_c->c_col = 0;
		cur_c->c_row++;
	}
	while (cur_c->c_row >= CON_ROWS)
	{
		scroll();
		cur_c->c_row--;
	}
	update_cursor(cur_c->c_row, cur_c->c_col);
}

void kputs(char* s)
{
	while (*s != 0)
	{
		kputc(*s);
		s++;
	}
}
