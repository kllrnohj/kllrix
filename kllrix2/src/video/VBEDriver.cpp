/*
 * VBEDriver.cpp
 *
 *  Created on: Oct 3, 2008
 *      Author: kllrnohj
 */

#include "VBEDriver.h"
#include "kernel.h"

namespace VBEDriver {

struct vbe_mode* mode;
struct vbe_controller* info;
volatile unsigned char* screen;

void Init()
{
	mode = (struct vbe_mode*)g_mbinfo->vbe_mode_info;
	info = (struct vbe_controller*)g_mbinfo->vbe_control_info;
	screen = (unsigned char*) mode->phys_base;
}


void FillScreen(int color)
{
	int size = mode->x_resolution * mode->y_resolution * (mode->bits_per_pixel / 8);
	volatile unsigned char* scr = (unsigned char*)(mode->phys_base - 1);
	for (int i = 0; i < size; i += (mode->bits_per_pixel / 8))
	{
		if (mode->bits_per_pixel >= 8)
			*(++scr) = color & 0xFF;
		if (mode->bits_per_pixel >= 16)
			*(++scr) = (color >> 8) & 0xFF;
		if (mode->bits_per_pixel >= 24)
			*(++scr) = (color >> 16) & 0xFF;
	}
}

void Scroll(int px, int fillcolor, enum DIRECTION dir)
{
	// TODO: Stop assuming DIRECTION is UP :)
	volatile unsigned int* scr = (unsigned int*)(mode->phys_base);
	for (int r = 0; r < mode->y_resolution - px; r++)
	{
		for (int c = 0; c < mode->x_resolution; c++)
		{
			*(scr) = *(scr + px*(mode->x_resolution * 3 / 4));
			scr++;
		}
	}
	for (int i = 0; i < (mode->x_resolution * px);)
	{
		if (mode->bits_per_pixel >= 8)
			screen[i++ + (mode->y_resolution - px) * mode->x_resolution] = fillcolor & 0xFF;
		if (mode->bits_per_pixel >= 16)
			screen[i++ + (mode->y_resolution - px) * mode->x_resolution] = (fillcolor >> 8) & 0xFF;
		if (mode->bits_per_pixel >= 24)
			screen[i++ + (mode->y_resolution - px) * mode->x_resolution] =(fillcolor >> 16) & 0xFF;
	}
}

int RGBToInt(int r, int g, int b)
{
	int color = 0;
	int rmask = 0, bmask = 0, gmask = 0;
	for (int i = 0; i < mode->red_mask_size; i++)
		rmask = (rmask << 1) + 1;
	for (int i = 0; i < mode->blue_mask_size; i++)
		bmask = (bmask << 1) + 1;
	for (int i = 0; i < mode->green_mask_size; i++)
		gmask = (gmask << 1) + 1;
	color |= (r & rmask) << mode->red_field_position;
	color |= (g & gmask) << mode->green_field_position;
	color |= (b & bmask) << mode->blue_field_position;
	return color;
}

void PutPixel(int x, int y, int color)
{
	int t = (x + (y * mode->x_resolution)) * (mode->bits_per_pixel / 8);
	if (mode->bits_per_pixel >= 8)
		screen[t] = color & 0xFF;
	if (mode->bits_per_pixel >= 16)
		screen[++t] = (color >> 8) & 0xFF;
	if (mode->bits_per_pixel >= 24)
		screen[++t] = (color >> 16) & 0xFF;
}

}
