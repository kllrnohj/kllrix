/*
 * PCIDriver.h
 *
 *  Created on: Oct 5, 2008
 *      Author: kllrnohj
 */

#ifndef PCIDRIVER_H_
#define PCIDRIVER_H_

#include "kernel.h"
#include "drivers/bus/PCIDriverManager.h"

#define  PCI_BASE_ADDRESS_SPACE		0x01
#define  PCI_BASE_ADDRESS_MEM_MASK	(~0x0fUL)
#define  PCI_BASE_ADDRESS_IO_MASK	(~0x03UL)

struct
{
	uintptr_t start, end;
	uint32_t flags;
} typedef resource;

// this is missing some info, but neither driver uses it, so the rest can fuck off
struct
{
	uint8_t bus, slot;
	uint16_t device_id, vendor_id;
	resource resources[6];
	uint8_t interrupt_pin, interrupt_line;
} typedef pci_dev;

class PCIDriver {
public:
	PCIDriver(int bus, int slot);

protected:
	pci_dev *m_dev;
};

#endif /* PCIDRIVER_H_ */
