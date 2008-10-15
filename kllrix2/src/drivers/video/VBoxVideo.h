/*
 * VBoxVideo.h
 *
 *  Created on: Oct 5, 2008
 *      Author: kllrnohj
 */

#ifndef VBOXVIDEO_H_
#define VBOXVIDEO_H_

#include "kernel.h"

#define VBE_DISPI_IOPORT_INDEX          0x01CE
#define VBE_DISPI_IOPORT_DATA           0x01CF
#define VBE_DISPI_INDEX_ID              0x0
#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2
#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4
#define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
#define VBE_DISPI_ID2                   0xB0C2
#define VBE_DISPI_DISABLED              0x00
#define VBE_DISPI_ENABLED               0x01
#define VBE_DISPI_LFB_ENABLED           0x40

class VBoxVideo {
public:
	VBoxVideo();
	virtual ~VBoxVideo();

private:
	uint32_t m_video_ram;
};

#endif /* VBOXVIDEO_H_ */
