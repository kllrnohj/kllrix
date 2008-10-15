/*
 * VesaVideo.cpp
 *
 *  Created on: Oct 8, 2008
 *      Author: kllrnohj
 */

#include "VesaVideo.h"

VesaVideo::VesaVideo(struct vbe_controller* controller, struct vbe_mode* mode) : m_controller(controller), m_mode(mode) {
	m_font = fonts[VESA_FONT];
}

VesaVideo::~VesaVideo() {
	// TODO Auto-generated destructor stub
}

uint32_t VesaVideo::RGBToUint(uint32_t r, uint32_t g, uint32_t b)
{
	int color = 0;
	int rmask = 0, bmask = 0, gmask = 0;
	for (int i = 0; i < m_mode->red_mask_size; i++)
		rmask = (rmask << 1) + 1;
	for (int i = 0; i < m_mode->blue_mask_size; i++)
		bmask = (bmask << 1) + 1;
	for (int i = 0; i < m_mode->green_mask_size; i++)
		gmask = (gmask << 1) + 1;
	color |= (r & rmask) << m_mode->red_field_position;
	color |= (g & gmask) << m_mode->green_field_position;
	color |= (b & bmask) << m_mode->blue_field_position;
	return color;
}

void VesaVideo::Clear(uint32_t color)
{
	int size = m_mode->x_resolution * m_mode->y_resolution;
	// if 32bit, use uints to speed this up
	if (m_mode->bits_per_pixel == 32)
	{
		volatile unsigned int* scr = (unsigned int*)(m_mode->phys_base - 4);
		for (int i = 0; i < size; i++)
			*(++scr) = color;
	}
	else if (m_mode->bits_per_pixel == 16)
	{
		volatile unsigned short* scr = (unsigned short*)(m_mode->phys_base - 2);
		for (int i = 0; i < size; i++)
			*(++scr) = (unsigned short)color;
	}
	else
	{
		volatile unsigned char* scr = (unsigned char*)(m_mode->phys_base - 1);
		for (int i = 0; i < size; i += (m_mode->bits_per_pixel / 8))
		{
			*(++scr) = color & 0xFF;
			*(++scr) = (color >> 8) & 0xFF;
			if (m_mode->bits_per_pixel >= 24)
				*(++scr) = (color >> 16) & 0xFF;
			if (m_mode->bits_per_pixel > 24)
				scr += (m_mode->bits_per_pixel - 24) / 8;
		}
	}
}

void VesaVideo::PutCharAt(char c, int col, int row, int fg, int bg)
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
				PutPixel(x, y, fg);
			else
				PutPixel(x, y, bg);
			x++;
		}
		y++;
	}
}

void VesaVideo::PutPixel(uint32_t x, uint32_t y, uint32_t color)
{
	int t = (x + (y * m_mode->x_resolution)) * (m_mode->bits_per_pixel / 8);
	volatile unsigned char* screen = (unsigned char*)m_mode->phys_base;
	screen[t] = color & 0xFF;
	screen[++t] = (color >> 8) & 0xFF;
	if (m_mode->bits_per_pixel >= 24)
		screen[++t] = (color >> 16) & 0xFF;
}
