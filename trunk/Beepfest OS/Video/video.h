#ifndef __VIDEO_H__
#define __VIDEO_H__

void kprint(const char* str);
void kprint_putchar(int c);
void kgetXY(void);
void kprintSN(int num, unsigned int radix);
void kprintUN(unsigned int num, unsigned int radix);
void clrscr(void);
void putPixel(int x, int y, unsigned char color);

#endif

