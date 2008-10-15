/*
 * FBConsole.h
 *
 *  Created on: Oct 2, 2008
 *      Author: kllrnohj
 */

#ifndef FBCONSOLE_H_
#define FBCONSOLE_H_
#include "Fonts.h"
#include "VBEDriver.h"
#include "include/stdarg.h"

enum
{
	CUR_OFF,
	CUR_UNDERSCORE,
	CUR_BLOCK,
};

class FBConsole {
public:
	FBConsole();
	void Clear();
	void PutCAt(char c, int col, int row);
	void PutC(char c);
	void PutL(long i, int min_width = 0);
	void PutUL(unsigned long i, int min_width = 0);
	void PutULHex(unsigned long i, bool upper, int min_width = 0);
	void Blink();

	// Only supports specifiers and 1 character widths!!
	void Printf(const char* format, ...);

	void SetBGColor(int bg) { m_bgcolor = bg; }
	void SetFGColor(int fg) { m_fgcolor = fg; }
	int  GetBGColor() { return m_bgcolor; }
	int  GetFGColor() { return m_fgcolor; }

private:
	void zeroPad(int nz);

	int m_bgcolor;
	int m_fgcolor;
	int m_col;
	int m_row;
	int m_cursor;
	const struct font_desc* m_font;
	int m_blink;
};

#endif /* FBCONSOLE_H_ */
