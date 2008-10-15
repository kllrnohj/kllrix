/*
 * VideoDriver.h
 *
 *  Created on: Oct 6, 2008
 *      Author: kllrnohj
 */

#ifndef VIDEODRIVER_H_
#define VIDEODRIVER_H_

#include "include/multiboot.h"
#include "stdint.h"
#include "include/colors.h"

class VideoDriver {
public:
	virtual void Clear(uint32_t color) = 0;
	virtual void PutCharAt(char c, int col, int row, int fg, int bg) = 0;
	virtual uint32_t RGBToUint(uint32_t r, uint32_t g, uint32_t b) = 0;

	static VideoDriver* GetDriver(multiboot_info_t *mbt);
};

#endif /* VIDEODRIVER_H_ */
