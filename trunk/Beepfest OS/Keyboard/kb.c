#include "kb.h"
#include "../Kernel/kernel.h"
#include "../Video/video.h"
#include "../Boot/irq.h"
#include "../Console/console.h"

unsigned char keybuffer[256];
unsigned char bufferpos;
unsigned char lastCode;
int state;
char thisLine[512];
char lastLine[512];
int linePos;
unsigned char leftShift = 0;
unsigned char rightShift = 0;
unsigned char keyDown = 0;

#define ISSHIFT (leftShift || rightShift)

void setKeybState(int st){
	state = st;
	int x;
	for(x = 0; x < 256; x++){
		keybuffer[x] = 0;
	}
	for(x = 0; x < 512; x++){
		thisLine[x] = 0;
	}
	linePos = 0;
	lastCode = 0;
	bufferpos = 255;
}

void shiftBuffer(){
	int x;
	for(x = 255; x > 0; x--){
		keybuffer[x] = keybuffer[x - 1];
	}
	keybuffer[0] = 0;
	if(bufferpos < 255) bufferpos++;
	
}

void keyboard_handler(struct regs *r){
	unsigned char scancode;
	scancode = inb(0x60); // read the value
	
	if(scancode & 0x80){ // break code
		if(keyval[scancode & ~0x80] == LSHIFT){
			leftShift = 0;
		}else if(keyval[scancode & ~0x80] == RSHIFT){
			rightShift = 0;
		}
	}else{ // make code
		// if our state is CONSOLE, when we receive a
		// newline we should send the command and args
		// to the console function with the whole line
		switch(state){
			case CONSOLE:
			{
				unsigned char value = (ISSHIFT ? keyvalUpper[scancode] : keyval[scancode]);
				if(keyval[scancode] == LSHIFT){
					leftShift = 1;
					break;
				}else if(keyval[scancode] == RSHIFT){
					rightShift = 1;
					break;
				}
				if(value == '\b'){
					if(linePos > 0){ thisLine[--linePos] = 0; kprint_putchar(value); }
				}else{
					thisLine[linePos++] = value;
					kprint_putchar(value);
				}
				// now we end the function and pass it off to the console thingie
				// if the last char was a newline
				if(value == '\n' || linePos == 512){
					thisLine[linePos-1] = '\0';
					int p; 
					for(p = 0; p < 512; p++){
						lastLine[p] = thisLine[p];
						thisLine[p] = 0;
					}
					setToDoCP(lastLine);
					setToDo(doConsole);
					linePos = 0;
					
					
				}
			}break;
			case SPACEINVADERSKEYB:
			{
				//kprint_putchar(keyval[scancode]);
				if(bufferpos != 0) keybuffer[bufferpos--] = scancode;
			}break;
			case SNAKE:
			{
				keyDown = scancode;
			}break;
		}	
	}
}

void keyboard_install(){
	setKeybState(CONSOLE);
	irq_install_handler(1, keyboard_handler);
}

unsigned char getNextKeypress(unsigned char repeatKey){
	if(repeatKey || lastCode == 0){
		unsigned char result = keybuffer[255];
		shiftBuffer();
		return result;
	}else{
		unsigned char temp;
		do{			
			temp = keybuffer[255];
			shiftBuffer();
		}while(temp == lastCode);
		lastCode = temp;
		
		return temp;
	}
}
