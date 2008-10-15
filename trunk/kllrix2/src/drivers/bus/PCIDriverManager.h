/*
 * PCIDriverManager.h
 *
 *  Created on: Oct 5, 2008
 *      Author: kllrnohj
 */

#ifndef PCIDRIVERMANAGER_H_
#define PCIDRIVERMANAGER_H_

#include "kernel.h"
#include "drivers/dummy/PCIDriver.h"

class PCIDriverManager {
public:
	PCIDriverManager();
	virtual ~PCIDriverManager();
	// void for now
	void Probe(int bus = 0);

	static uint16_t pciConfigReadWord (unsigned short bus, unsigned short slot,
	                                  unsigned short func, unsigned short offset);
	static uint32_t pciConfigReadDword (unsigned short bus, unsigned short slot,
	                                  unsigned short func, unsigned short offset);

	static void pciConfigWriteWord (unsigned short bus, unsigned short slot,
	                                  unsigned short func, unsigned short offset, uint16_t value);
	static void pciConfigWriteDword (unsigned short bus, unsigned short slot,
	                                  unsigned short func, unsigned short offset, uint32_t value);
};

#endif /* PCIDRIVERMANAGER_H_ */
