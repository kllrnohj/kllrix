#ifndef KBD_H_
#define KBD_H_
#include "isrs.h"

#define L_SHIFT 1
#define R_SHIFT 2
#define SHIFT 3

extern unsigned long kbd_status;
extern unsigned char kbdus[256];
void keyboard_handler(struct regs *r);
void keyboard_install();

#endif /*KBD_H_*/
