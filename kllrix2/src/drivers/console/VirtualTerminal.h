/*
 * VirtualTerminal.h
 *
 *  Created on: Oct 6, 2008
 *      Author: kllrnohj
 */

#ifndef VIRTUALTERMINAL_H_
#define VIRTUALTERMINAL_H_

#include "drivers/dummy/VideoDriver.h"

#define BUFFER_SIZE (0x5000)

class VirtualTerminal {
public:
	VirtualTerminal(VideoDriver* video, uint32_t width, uint32_t height);
	virtual ~VirtualTerminal();
	void Printf(const char* frmt, ...);
	void SetBGColor(int bg) { m_bgcolor = bg; }
	void SetFGColor(int fg) { m_fgcolor = fg; }
	int  GetBGColor() { return m_bgcolor; }
	int  GetFGColor() { return m_fgcolor; }
	void Clear();

protected:
	void putC(char c);
	void putL(long i, int min_width = 0);
	void putUL(unsigned long i, int min_width = 0);
	void putULHex(unsigned long i, bool upper, int min_width = 0);
	void zeroPad(int nz);

	VideoDriver* m_video;
	uint32_t m_vtwidth, m_vtheight;
	char* m_buffer;
	uint32_t m_bgcolor;
	uint32_t m_fgcolor;
	uint32_t m_col;
	uint32_t m_row;
};

#endif /* VIRTUALTERMINAL_H_ */
