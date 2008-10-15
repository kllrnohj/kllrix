/*
 * cstdio
 *
 *  Created on: Oct 6, 2008
 *      Author: kllrnohj
 */

#ifndef KSTDIO_
#define KSTDIO_

#include "globals.h"

#define printf(...) Globals::Terminal->Printf(__VA_ARGS__)

#endif /* CSTDIO_ */
