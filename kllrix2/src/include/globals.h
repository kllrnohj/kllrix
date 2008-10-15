/*
 * globals.h
 *
 *  Created on: Oct 6, 2008
 *      Author: kllrnohj
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "drivers/console/VirtualTerminal.h"
#include "drivers/dummy/VideoDriver.h"

namespace Globals
{
	extern VideoDriver* Video;
	extern VirtualTerminal* Terminal;
}

#endif /* GLOBALS_H_ */
