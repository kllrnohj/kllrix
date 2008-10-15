/*
 * VideoDriver.cpp
 *
 *  Created on: Oct 6, 2008
 *      Author: kllrnohj
 */

#include "VideoDriver.h"
#include "drivers/video/TextVideo.h"
#include "drivers/video/VesaVideo.h"

VideoDriver* VideoDriver::GetDriver(multiboot_info_t *mbt)
{
	if (!(mbt->flags & MB_INFO_VIDEO_INFO) || !mbt->vbe_mode_info)
		return new TextVideo();
	return new VesaVideo((struct vbe_controller*)mbt->vbe_control_info, (struct vbe_mode*)mbt->vbe_mode_info);
}
