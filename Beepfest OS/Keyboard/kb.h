#ifndef __KB_H__
#define __KB_H__

#define CONSOLE 0
#define SPACEINVADERSKEYB 1
#define SNAKE 2

#define LSHIFT 1
#define RSHIFT 2

static unsigned char keyval[128] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=', '\b',
	'\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'[', ']', '\n', // \n is enter
	0, // control
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', LSHIFT, // left shift
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
	',', '.', '/', RSHIFT, // right shift
	'*', // on numlocked side
	0, // alt
	' ',
	0, // caps
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 - F10
	0, // num lock
	0, // scroll lock
	0, // home
	0, // up
	0, // page up
	'-',
	0, // left arrow
	0,
	0, // right arrow
	'+',
	0, // end key
	0, // down arrow
	0, // page down
	0, // insert key
	0, // delete
	0, 0, 0,
	0, // f11
	0, // f12
	0, // the rest are undefined
};

static unsigned char keyvalUpper[128] = {
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*',
	'(', ')', '_', '+', '\b',
	'\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'{', '}', '\n', // \n is enter
	0, // control
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
	'"', '~', LSHIFT, // left shift
	'|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',
	'<', '>', '?', RSHIFT, // right shift
	'*', // on numlocked side
	0, // alt
	' ',
	0, // caps
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 - F10
	0, // num lock
	0, // scroll lock
	0, // home
	0, // up
	0, // page up
	'-',
	0, // left arrow
	0,
	0, // right arrow
	'+',
	0, // end key
	0, // down arrow
	0, // page down
	0, // insert key
	0, // delete
	0, 0, 0,
	0, // f11
	0, // f12
	0, // the rest are undefined
};

#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define ARROW_UP 72
#define ARROW_DOWN 80

void keyboard_install();
unsigned char getNextKeypress(unsigned char repeatKey);
void setKeybState(int st);
unsigned char shiftOn();
extern unsigned char keyDown;

#endif

