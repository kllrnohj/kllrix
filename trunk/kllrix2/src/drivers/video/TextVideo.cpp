/*
 * TextVideo.cpp
 *
 *  Created on: Oct 6, 2008
 *      Author: kllrnohj
 */

#include "TextVideo.h"
#include "include/colors.h"

TextVideo::TextVideo() {
	m_video = (unsigned short*) VIDEO_ADDRESS_TEXT;
	DisableCursor();
}

TextVideo::~TextVideo() {
	// TODO Auto-generated destructor stub
}

uint32_t TextVideo::RGBToUint(uint32_t r, uint32_t g, uint32_t b)
{
	int color = 0;
	if (r - 20 < 255) color |= (1 << 2);
	if (g - 20 < 255) color |= (1 << 1);
	if (b - 20 < 255) color |= 1;
	if (r > 128 || g > 128 || b > 128) color |= (1 << 3);
	if (color == 7) return 8;
	if (color == 15)
		if (r < 230 || g < 230 || b < 230) return 7;
	return color;
}

void TextVideo::Clear(uint32_t color)
{
	for (int i = 0; i < (TEXT_HEIGHT*TEXT_WIDTH); i++)
	{
		m_video[i] = (uint16_t)(color << 12);
	}
	DisableCursor();
}

void TextVideo::PutCharAt(char c, int col, int row, int fg, int bg)
{
	uint16_t attrib = (bg & 0x0F) << 12;
	attrib |= (fg & 0x0F) << 8;
	attrib |= c;
	m_video[col + row*TEXT_WIDTH] = attrib;
}

void TextVideo::SetCursorLocation(int col, int row)
{
	   unsigned short position=(row*80) + col;

	   // cursor LOW port to vga INDEX register
	   outb(0x3D4, 0x0F);
	   outb(0x3D5, (unsigned char)(position&0xFF));
	   // cursor HIGH port to vga INDEX register
	   outb(0x3D4, 0x0E);
	   outb(0x3D5, (unsigned char )((position>>8)&0xFF));

}
