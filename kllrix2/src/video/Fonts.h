/*
 * Font8x8.h
 *
 *  Created on: Oct 2, 2008
 *      Author: kllrnohj
 */

#ifndef FONT8X8_H_
#define FONT8X8_H_

struct font_desc {
    int idx;
    const char *name;
    int width, height;
    const unsigned char *data;
    int pref;
};

#define VGA8x8_IDX	0
#define VGA8x16_IDX	1
#define VGA_MINI4x6_IDX 2
#define VGA6x11_IDX 3
#define VGA7x14_IDX 4

extern const struct font_desc	font_vga_8x8,
								font_vga_8x16,
								font_mini_4x6,
								font_vga_6x11,
								font_vga_7x14;

static const struct font_desc* fonts[] = {&font_vga_8x8, &font_vga_8x16, &font_mini_4x6, &font_vga_6x11, &font_vga_7x14};

#endif /* FONT8X8_H_ */
