/*
 * VBoxVideo.cpp
 *
 *  Created on: Oct 5, 2008
 *      Author: kllrnohj
 */

#include "VBoxVideo.h"

VBoxVideo::VBoxVideo() {
	m_video_ram = inl(VBE_DISPI_IOPORT_DATA);

}

VBoxVideo::~VBoxVideo() {
	// TODO Auto-generated destructor stub
}
