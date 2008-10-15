/*
 * VesaVideo.h
 *
 *  Created on: Oct 8, 2008
 *      Author: kllrnohj
 */

#ifndef VESAVIDEO_H_
#define VESAVIDEO_H_

#include "kernel.h"
#include "drivers/dummy/VideoDriver.h"
#include "video/Fonts.h"
#include "include/colors.h"

#define VESA_FONT VGA8x16_IDX
#define VESA_CURSOR '_'

class VesaVideo : public VideoDriver {
public:
	VesaVideo(struct vbe_controller* controller, struct vbe_mode* mode);
	virtual ~VesaVideo();
	void Clear(uint32_t color);
	void PutCharAt(char c, int col, int row, int fg, int bg);
	void PutPixel(uint32_t x, uint32_t y, uint32_t color);
	uint32_t RGBToUint(uint32_t r, uint32_t g, uint32_t b);

protected:
	struct vbe_controller* m_controller;
	struct vbe_mode* m_mode;
	const struct font_desc* m_font;
};

#endif /* VESAVIDEO_H_ */
