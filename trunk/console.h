#ifndef CONSOLE_H_
#define CONSOLE_H_

#define CON_COLUMNS 80
#define CON_ROWS 25

void kputc(char c);
void kputs(char* s);
void kcls();
void kputi(int i);

typedef struct {
	unsigned short *buffer;
	int c_row;
	int c_col;
	int c_fore;
	int c_back;
} console_s;

extern console_s* cur_c;

#endif /*CONSOLE_H_*/
