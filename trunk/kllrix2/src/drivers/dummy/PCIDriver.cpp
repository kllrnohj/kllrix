/*
 * PCIDriver.cpp
 *
 *  Created on: Oct 5, 2008
 *      Author: kllrnohj
 */

#include "PCIDriver.h"

uint32_t pci_size(uint32_t base, uint32_t maxbase, uint32_t mask)
{
	uint32_t size = mask & maxbase;	/* Find the significant bits */
	if (!size)
		return 0;

	/* Get the lowest of them to find the decode size, and
	   from that the extent.  */
	size = (size & ~(size-1)) - 1;

	/* base == maxbase can be valid only if the BAR has
	   already been programmed with all 1s.  */
	if (base == maxbase && ((base | size) & mask) != mask)
		return 0;

	return size;
}

PCIDriver::PCIDriver(int bus, int slot)
{
	m_dev = new pci_dev;
	m_dev->bus = bus;
	m_dev->slot = slot;
	int l = PCIDriverManager::pciConfigReadDword(bus, slot, 0, 0x0);
	m_dev->vendor_id = (l & 0xFFFF);
	m_dev->device_id = (l >> 16) & 0xFFFF;
	l = PCIDriverManager::pciConfigReadDword(bus, slot, 0, 0x3c);
	m_dev->interrupt_line = l & 0xFF;
	m_dev->interrupt_pin = (l >> 8) & 0xFF;

	// lets process some resources!
	for (l = 0x10; l <= 0x24; l+= 4)
	{
		int data = PCIDriverManager::pciConfigReadDword(bus, slot, 0, l);
		PCIDriverManager::pciConfigWriteDword(bus, slot, 0, l, ~0);
		uint32_t sz = PCIDriverManager::pciConfigReadDword(bus, slot, 0, l);
		PCIDriverManager::pciConfigWriteDword(bus, slot, 0, l, data);
		uint32_t size;
		if (data & PCI_BASE_ADDRESS_SPACE)
		{
			size = pci_size(data, sz, PCI_BASE_ADDRESS_IO_MASK & 0xffff);
			m_dev->resources[(l - 0x10) / 4].start = data & PCI_BASE_ADDRESS_IO_MASK;
			m_dev->resources[(l - 0x10) / 4].flags = data & ~PCI_BASE_ADDRESS_IO_MASK;
		}
		else
		{
			size = pci_size(data, sz,  (uint32_t)PCI_BASE_ADDRESS_MEM_MASK);
			m_dev->resources[(l - 0x10) / 4].start = data & PCI_BASE_ADDRESS_MEM_MASK;
			m_dev->resources[(l - 0x10) / 4].flags = data & ~PCI_BASE_ADDRESS_MEM_MASK;
		}
		m_dev->resources[(l - 0x10) / 4].end = m_dev->resources[(l - 0x10) / 4].start + size;
	}
}
