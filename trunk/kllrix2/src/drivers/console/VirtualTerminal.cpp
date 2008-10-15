/*
 * VirtualTerminal.cpp
 *
 *  Created on: Oct 6, 2008
 *      Author: kllrnohj
 */

#include "VirtualTerminal.h"
#include "stdarg.h"

VirtualTerminal::VirtualTerminal(VideoDriver* video, uint32_t width, uint32_t height) : m_video(video), m_vtwidth(width), m_vtheight(height) {
	m_buffer = new char[BUFFER_SIZE];
	m_fgcolor = video->RGBToUint(0, 240, 0);
	m_bgcolor = video->RGBToUint(0, 0, 0);
	Clear();
}

VirtualTerminal::~VirtualTerminal() {
	// TODO Auto-generated destructor stub
}

void VirtualTerminal::putC(char c)
{
	switch (c)
	{
	case 0: break;
	case '\n': m_video->PutCharAt(' ', m_col, m_row, m_fgcolor, m_bgcolor); m_col = 0; m_row++; break;
	case '\t': m_video->PutCharAt(' ', m_col, m_row, m_fgcolor, m_bgcolor); m_col += (8 - m_col % 8); break;
	case '\r': m_col = 0; break;
	case '\b':
		m_video->PutCharAt(' ', m_col, m_row, m_fgcolor, m_bgcolor);
		if (m_col > 0)
			m_col--;
		m_video->PutCharAt(' ', m_col, m_row, m_fgcolor, m_bgcolor);
		break;
	default:
		if (c < ' ') return; // don't know what to do with this :)
		m_video->PutCharAt(c, m_col, m_row, m_fgcolor, m_bgcolor);
		m_col++;
	}
	if (m_col >= m_vtwidth)
	{
		m_col = 0;
		m_row++;
	}

	if (m_row >= m_vtheight)
	{
		m_row--;
		// todo: scroll
	}
}

// helper function
void VirtualTerminal::zeroPad(int nz)
{
	for (int i = 0; i < nz; i++)
		putC('0');
}

void VirtualTerminal::putL(long i, int min_width)
{
	if (i < 0)
	{
		putC('-');
		putUL(0 - i, min_width);
	} else {
		putUL(i, min_width);
	}
}

void VirtualTerminal::putUL(unsigned long i, int min_width)
{
	int d = i % 10;
	if (i >= 10) putUL(i / 10, min_width - 1);
	else zeroPad(min_width - 1);
	putC('0' + d);
}

void VirtualTerminal::putULHex(unsigned long i, bool upper, int min_width)
{
	int d = i % 16;
	if (i >= 16)
		putULHex(i / 16, upper, min_width - 1);
	else
		zeroPad(min_width - 1);
	if (d < 10) putC('0' + d);
	else
		if (upper) putC('7' + d);
		else putC('W' + d);
}

void VirtualTerminal::Printf(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	// TODO: Do more than just print out the format, lol
	while (*format != 0)
	{
		if (*format == '%')
		{
			format++;
			int width = 0;
			if (*format == '#')
			{
				Printf("0x");
				format++;
			}
			if (*format >= '0' && *format <= '9')
			{
				width = *format - '0';
				format++;
			}
			switch (*format)
			{
			case 'd':
			case 'i':
				putL((int)va_arg(vl, int), width);
				break;
			case 's':
				Printf((char*)va_arg(vl, char*));
				break;
			case 'c':
				putC((char)va_arg(vl, int));
				break;
			case 'x':
				putULHex((unsigned long)va_arg(vl, unsigned long), false, width);
				break;
			case 'X':
				putULHex((unsigned long)va_arg(vl, unsigned long), true, width);
				break;
			default:
				putC(*format);
			}
		} else {
			putC(*format);
		}
		format++;
	}

	va_end(vl);
}

void VirtualTerminal::Clear()
{
	// wipe the screen and reset console
	m_col = 0;
	m_row = 0;
	m_video->Clear(m_bgcolor);
}
