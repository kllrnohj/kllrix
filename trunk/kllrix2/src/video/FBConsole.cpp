/*
 * FBConsole.cpp
 *
 *  Created on: Oct 2, 2008
 *      Author: kllrnohj
 */

#include "FBConsole.h"

FBConsole::FBConsole() {
	m_bgcolor = VBEDriver::RGBToInt(0, 0, 0);
	m_fgcolor = VBEDriver::RGBToInt(0, 240, 0);
	m_col = 0;
	m_row = 0;
	m_font = fonts[VGA8x16_IDX];
	m_cursor = CUR_BLOCK;
	PutCAt('_', m_col, m_row);
	m_blink = 0;
}

void FBConsole::PutCAt(char c, int col, int row)
{
	int x;
	int y = row * m_font->height + 1;
	int fix = (int)((unsigned char)c) * m_font->height;
	for (int iy = 0; iy < m_font->height; iy++)
	{
		x = col * m_font->width + 1;
		unsigned char l = m_font->data[fix + iy];
		for (int b = m_font->width - 1; b >= 0; b--)
		{
			if ((l >> b) & 0x01)
				VBEDriver::PutPixel(x, y, m_fgcolor);
			else
				VBEDriver::PutPixel(x, y, m_bgcolor);
			x++;
		}
		y++;
	}
}

void FBConsole::PutC(char c)
{
	switch (c)
	{
	case 0: break;
	case '\n': PutCAt(' ', m_col, m_row); m_col = 0; m_row++; break;
	case '\t': PutCAt(' ', m_col, m_row); m_col += (8 - m_col % 8); break;
	case '\r': m_col = 0; break;
	case '\b':
		PutCAt(' ', m_col, m_row);
		if (m_col > 0)
			m_col--;
		PutCAt(' ', m_col, m_row);
		break;
	default:
		if (c < ' ') return; // don't know what to do with this :)
		PutCAt(c, m_col, m_row);
		m_col++;
	}
	if (m_col >= (VBEDriver::mode->x_resolution / m_font->width))
	{
		m_col = 0;
		m_row++;
	}

	if (m_row >= (VBEDriver::mode->y_resolution / m_font->height))
	{
		m_row--;
		VBEDriver::Scroll(m_font->height, m_bgcolor);
	}

	if (m_cursor)
	{
		switch (m_cursor)
		{
		case CUR_UNDERSCORE:
			PutCAt('_', m_col, m_row);
			break;
		case CUR_BLOCK:
			PutCAt(219, m_col, m_row);
			break;
		}
	}
}

void FBConsole::Blink()
{
	if (m_cursor)
	{
		if (m_blink)
			PutCAt(' ', m_col, m_row);
		else
			PutC(0);
		m_blink = !m_blink;
	}
}

// helper function
void FBConsole::zeroPad(int nz)
{
	for (int i = 0; i < nz; i++)
		PutC('0');
}

void FBConsole::PutL(long i, int min_width)
{
	if (i < 0)
	{
		PutC('-');
		PutUL(0 - i, min_width);
	} else {
		PutUL(i, min_width);
	}
}

void FBConsole::PutUL(unsigned long i, int min_width)
{
	int d = i % 10;
	if (i >= 10) PutUL(i / 10, min_width - 1);
	else zeroPad(min_width - 1);
	PutC('0' + d);
}

void FBConsole::PutULHex(unsigned long i, bool upper, int min_width)
{
	int d = i % 16;
	if (i >= 16)
		PutULHex(i / 16, upper, min_width - 1);
	else
		zeroPad(min_width - 1);
	if (d < 10) PutC('0' + d);
	else
		if (upper) PutC('7' + d);
		else PutC('W' + d);
}

void FBConsole::Printf(const char* format, ...)
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
				PutL((int)va_arg(vl, int), width);
				break;
			case 's':
				Printf((char*)va_arg(vl, char*));
				break;
			case 'c':
				PutC((char)va_arg(vl, int));
				break;
			case 'x':
				PutULHex((unsigned long)va_arg(vl, unsigned long), false, width);
				break;
			case 'X':
				PutULHex((unsigned long)va_arg(vl, unsigned long), true, width);
				break;
			default:
				PutC(*format);
			}
		} else {
			PutC(*format);
		}
		format++;
	}

	va_end(vl);
}

void FBConsole::Clear()
{
	// wipe the screen and reset console
	m_col = 0;
	m_row = 0;
	VBEDriver::FillScreen(m_bgcolor);
}
