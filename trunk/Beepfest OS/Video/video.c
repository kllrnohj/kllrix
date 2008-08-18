#include "video.h"

static unsigned int screen_x = 0;
static unsigned int screen_y = 0;
static unsigned char* VGA = (unsigned char*)0xA0000;

void setMode(unsigned char mode);

void kgetXY(void){
	unsigned char* biosxy = (unsigned char*)0x450;
	screen_x = 0;
	screen_y = (*(biosxy + 1)) + 1;
}

static void copy(void* dest, void* src, int len){
	int x;
	for(x = 0; x < len; x++){
		((char*)dest)[x] = ((char*)src)[x];
	}
}

static void kprint_scroll(void){
	// this functions scrolls the display upwards

	// so start with moving the previous text up
	copy((void*)0xB8000, (void*)(0xB8000 + 160), 3840);
	// 0xB8000 is video memory
	// 0xB8000 + 160 = 80 * 2 because each character is a byte,
	// then its attribute is another byte, display is 80 characters wide
	// 3840 = 160 * 24 where 24 is height of display minus 1, 160 like before

	// clear bottom line
	unsigned int* tmp = (unsigned int*)(0xB8000 + (160 * 24));
	unsigned int i;
	for(i = 0; i < (80 / 2); i++){
		// i is 4 bytes, so thats why 80/2, we do two at once
		tmp[i] = ((7 | ' ') << 16) | (7 | ' '); // see? 4 bytes
	}
}

void kprint_putchar(int c){
	switch(c){
		case 0:
		{
		}return;
		case '\b':
		{
			int x = 1;
			unsigned char doLoop = 0;
			if(*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x-2) == 0)
				doLoop = 1;
			while((*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x-2) == 0 && doLoop) || x-- == 1){
				if(screen_x > 0){
					screen_x -= 2;
					*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x) = 0;
				}else if(screen_x == 0 && screen_y == 0){
					// do nothing, because there are no characters
					break;
				}else{
					screen_x = 158;
					screen_y--;
					*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x) = 0;
				}
			}
		}break;
		case '\n':
		{
			screen_x = 0;
			if(screen_y == 24)
				kprint_scroll();
			else
				screen_y++;
		}break;
		case '\t':
		{
			screen_x += 4-(screen_x % 4);
			if(screen_x > 158){
				screen_x = 0;
				if(screen_y == 24)
					kprint_scroll();
				else
					screen_y++;	
			}
		}break;
		default:
		{
			*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x) = (c | (7<<8)); // 7 << 8 is for attribute
			screen_x += 2;
	
			if(screen_x > 158){
				screen_x = 0;
		
				if(screen_y == 24){
					kprint_scroll();
				}else{
					screen_y++;
				}
			}
		}break;
	}
}

void kprint(const char* str){ // eventually add argument list
	for(; *str != 0; str++){
		kprint_putchar(*str);
	}
}

void kprintSN(int num, unsigned int radix){
	if(num < 0)
		kprint_putchar('-');
	unsigned int realNum = -num;

	char buffer[33];
	char* where = &buffer[32]; // point to last spot in buffer
	
	*where = 0; // nullify last position

	do{
		unsigned int temp;
		temp = realNum % radix;
		where--; // do this first because we start at last which is now \0
		if(temp < 10)
			*where = temp + '0';
		else
			*where = (temp - 10) + 'A'; // hex

		realNum = (unsigned int)(realNum / radix);
	}while(realNum != 0);

	while(*where != 0){
		kprint_putchar(*where);
		where++;
	}
}

void kprintUN(unsigned int num, unsigned int radix){
	unsigned int realNum = num;

	char buffer[33];
	char* where = &buffer[32]; // point to last spot in buffer
	
	*where = 0; // nullify last position

	do{
		unsigned int temp;
		temp = realNum % radix;
		where--; // do this first because we start at last which is now \0
		if(temp < 10)
			*where = temp + '0';
		else
			*where = (temp - 10) + 'A'; // hex

		realNum = (unsigned int)(realNum / radix);
	}while(realNum != 0);

	while(*where != 0){
		kprint_putchar(*where);
		where++;
	}
}

void clrscr(void){
	// 25 rows by 80 cols
	// 2 bytes a char
	screen_x = 0;
	screen_y = 0;

	unsigned char temp = 0;
	unsigned int x;
	unsigned char* videoRam = (unsigned char*)0xB8000;
	for(x = 0; x < (25 * 80 * 2); x+=2){
		videoRam[x] = temp;
		videoRam[x+1] = temp;
	}
}

void putPixel(int x, int y, unsigned char color){
	/* y << 8 = y * (2 ^ 8) and y << 6 = y * (2^6) = y * 320 (320 * 200) */
	VGA[(y << 8) + (y << 6) + x] = color;
}

void setMode(unsigned char mode){
	/*
		0x3C0 address -> 0x3C1 data
		0x3C2 is write only for MOR, read at 0x3CC
		0x3C4 address -> 0x3C5 data
		0x3CE address -> 0x3CF data
		0x3D4 address -> 0x3D5 data
	*/
	/* set mode to 13h */
	unsigned char throwaway = in(0x3DA);
	out(0x3C0, 0x10);
	out(0x3C0, 0x41);
	
	throwaway = in(0x3DA);
	out(0x3C0, 0x11);
	out(0x3C0, 0x00);
	
	throwaway = in(0x3DA);
	out(0x3C0, 0x12);
	out(0x3C0, 0x0F);
	
	throwaway = in(0x3DA);
	out(0x3C0, 0x13);
	out(0x3C0, 0x00);
	
	throwaway = in(0x3DA);
	out(0x3C0, 0x14);
	out(0x3C0, 0x00);
	
	out(0x3C2, 0x63);
	
	out(0x3C4, 0x01);
	out(0x3C5, 0x01);
	
	out(0x3C4, 0x03);
	out(0x3C5, 0x00);
	
	out(0x3C4, 0x04);
	out(0x3C5, 0x0E);
	
	out(0x3CE, 0x05);
	out(0x3CF, 0x40);
	
	out(0x3CE, 0x06);
	out(0x3CF, 0x05);
	
	out(0x3D4, 0x11);
	out(0x3D5, 0x8E);
	
	out(0x3D4, 0x00);
	out(0x3D5, 0x5F);
	out(0x3D4, 0x01);
	out(0x3D5, 0x4F);
	out(0x3D4, 0x02);
	out(0x3D5, 0x50);
	out(0x3D4, 0x03);
	out(0x3D5, 0x82);
	out(0x3D4, 0x04);
	out(0x3D5, 0x54);
	out(0x3D4, 0x05);
	out(0x3D5, 0x80);
	out(0x3D4, 0x06);
	out(0x3D5, 0xBF);
	out(0x3D4, 0x07);
	out(0x3D5, 0x1F);
	out(0x3D4, 0x08);
	out(0x3D5, 0x00);
	out(0x3D4, 0x09);
	out(0x3D5, 0x41);
	out(0x3D4, 0x10);
	out(0x3D5, 0x9C);
	
	out(0x3D4, 0x12);
	out(0x3D5, 0x8F);
	out(0x3D4, 0x13);
	out(0x3D5, 0x28);
	out(0x3D4, 0x14);
	out(0x3D5, 0x40);
	out(0x3D4, 0x15);
	out(0x3D5, 0x96);
	out(0x3D4, 0x16);
	out(0x3D5, 0xB9);
	out(0x3D4, 0x17);
	out(0x3D5, 0xA3);
	/*
	/* CHANGE GRAPHICS MODE 
	out(0x3CE, 0x05); // 0x3CE is address register, 0x05 is Graphics Mode Register
	//unsigned char lol = in(0x3CF); // DEBUG STUFF
	//kprintUN(lol, 16); // DEBUG STUFF
	out(0x3CF, mode); // 0x3CF is data register, mode is video mode... hopefully.
	
	/* SET BIT MASK FOR & TO FF 
	out(0x3CE, 0x08); // 0x3CE is address register, 0x08 is Bit Mask register
	out(0x3CF, 0xFF); // 0x3CF is data register, 0xFF is filled bit field
	
	/* SET COLOR DON'T CARE REGISTER  /* DONT KNOW WHAT THIS DOES EXACTLY 
	out(0x3CE, 0x07); // 0x3CE is address register, 0x07 is Color Don't Care Register
	out(0x3CF, 0x0F); // SAY CHECK ALL PLANES (ONLY FIRST  4 BITS USED)
	
	/* SELECT MEMORY MAP AND DISABLE ALPHANUMERIC MODE (first bit is alphanumeric mode disable 
	out(0x3CE, 0x06); // 0x3CE is address register, 0x06 is Miscellaneous Graphics Register
	out(0x3CF, 0x01); // data register... bits 2-3 (from 0) are memory map select (00b: 128K region at A0000) bit 1 is Chain O/E (don't know what it does so i don't set it), disable alphanumeric mode.
	
	/* SET 0th PLANE AS OUR DEFAULT 
	out(0x3CE, 0x04); // 0x3CE is address register, 0x04 is Read Map Select Register
	out(0x3CF, 0x00); // use plane 0
	
	/* LOGICAL OPERATION IN 0x03 IS NOT USED, SO I ZERO IT, ROTATE COUNT WILL BE 0, SO MODE IS 0 
	out(0x3CE, 0x03); // 0x3CE is address register, 0x03 is Data Rotate Register
	out(0x3CF, 0x01); // turn off rotation and the logical operation
	
	/* COLOR COMPARE REGISTER, JUST SET ALL PLANES, EVEN THOUGH IM JUST USING ONE 
	out(0x3CE, 0x02); // 0x3CE is address register, 0x02 is Color Compare Register
	out(0x3CF, 0x0F); // 0x0F sets the first four bits, thus enabling the four planes
	
	/* WE DONT CARE ABOUT THIS REGISTER (Enable, Set/Reset Register [since it is only used in graphics mode 0 and we're in 3]) so I'll just enable all planes again 
	out(0x3CE, 0x01); // 0x3CE is address register, 0x01 is enable set/reset register
	out(0x3CF, 0x0F); // 0x0F for first four bits, four planes
	
	/* IMPORTANT Set/Reset Register used in mode 3, we set mode 0x0F to enable the planes 
	out(0x3CE, 0x00); // 0x3CE is address register, 0x00 is Set/Reset Register
	out(0x3CF, 0x0F); // 0x0F is first four bits, four planes
	
	
	/*unsigned char temp = in(0x3DA);
	out(0x3D4, 0xD);
	temp = in(0x3B4);
	out(0x3D4, 0xC);
	temp |= (in(0x3B4) << 8);
	//out(0x3C0, 0x10);
	//temp = 10;//in(0x3C0);
	kprintUN(temp, 16);
	temp &= 0xFE;
	//out(0x3c0, temp);*/
	if(mode == 0x13){
		int x;
		for(x = 0; x < 64000; x++)
			VGA[x] = (x % 256);
	}
	//__asm__ __volatile__ (
	//	"movb $0x00, %%ah\n\t"
	//	"movb %0, %%al\n\t"
	//	"int $0x10" : : "r"(mode) : "%ah", "%al");
}
