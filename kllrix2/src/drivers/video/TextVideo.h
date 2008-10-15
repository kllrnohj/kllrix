/*
 * TextVideo.h
 *
 *  Created on: Oct 6, 2008
 *      Author: kllrnohj
 */

#ifndef TEXTVIDEO_H_
#define TEXTVIDEO_H_

#include "kernel.h"
#include "drivers/dummy/VideoDriver.h"

#define VIDEO_ADDRESS_TEXT (0xB8000 + KERNEL_VIRTUAL_BASE)
#define TEXT_WIDTH 80
#define TEXT_HEIGHT 25

class TextVideo : public VideoDriver {
public:
	TextVideo();
	virtual ~TextVideo();
	void Clear(uint32_t color);
	void PutCharAt(char c, int col, int row, int fg, int bg);
	void SetCursorLocation(int col, int row);
	void DisableCursor() { SetCursorLocation(TEXT_WIDTH, TEXT_HEIGHT); }
	uint32_t RGBToUint(uint32_t r, uint32_t g, uint32_t b);
private:
	volatile unsigned short* m_video;
};

#endif /* TEXTVIDEO_H_ */
