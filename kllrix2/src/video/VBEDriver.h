/*
 * VBEDriver.h
 *
 *  Created on: Oct 3, 2008
 *      Author: kllrnohj
 */

#ifndef VBEDRIVER_H_
#define VBEDRIVER_H_
#include "include/multiboot.h"

namespace VBEDriver {

enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

extern struct vbe_mode *mode;
void Init();
int RGBToInt(int r, int g, int b);
void FillScreen(int color);
void PutPixel(int x, int y, int color);
void Scroll(int px, int fillcolor, enum DIRECTION dir = UP);

}

#endif /* VBEDRIVER_H_ */
