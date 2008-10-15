/*
 * PCIDriverManager.cpp
 *
 *  Created on: Oct 5, 2008
 *      Author: kllrnohj
 */

#include "PCIDriverManager.h"

PCIDriverManager::PCIDriverManager() {
}

PCIDriverManager::~PCIDriverManager() {
	// TODO Auto-generated destructor stub
}

extern uint32_t pci_size(uint32_t base, uint32_t maxbase, uint32_t mask);
void PCIDriverManager::Probe(int bus)
{
	// for each slot
	for (int slot = 0; slot < 32; slot++)
	{
		unsigned short vendor,device;
		/* try and read the first configuration register. Since there are no */
		/* vendors that == 0xFFFF, it must be a non-existent device. */
		vendor = pciConfigReadWord(bus,slot,0,0);
		if (vendor == 0xffff || vendor == 0x0000)
			continue;
		device = pciConfigReadWord(bus,slot,0,2);
		printf("Found PCI Device: %4x:%4x in slot %d\n", (int)vendor, (int)device, slot);
		if (device == 0xbeef || device == 0xcafe)
		{
			for (int l = 0; l <= 0x3c; l+= 4)
			{
				int data = pciConfigReadDword(bus, slot, 0, l);
				if (l >= 0x10 && l <= 0x24)
				{
					printf("\tBAR%d: %8x -> ", (l - 0x10) / 4, data);
					pciConfigWriteDword(bus, slot, 0, l, ~0);
					uint32_t sz = pciConfigReadDword(bus, slot, 0, l);
					pciConfigWriteDword(bus, slot, 0, l, data);
					if (data & PCI_BASE_ADDRESS_SPACE)
						printf("%8x (%x)\n", sz, pci_size(data, sz, PCI_BASE_ADDRESS_IO_MASK & 0xffff));
					else
						printf("%8x (%x)\n", sz, pci_size(data, sz,  (uint32_t)PCI_BASE_ADDRESS_MEM_MASK));
				}
				else if (data)
					printf("\t%2x: %8x\n", l, data);
			}
		}
	}
}

uint16_t PCIDriverManager::pciConfigReadWord (unsigned short bus, unsigned short slot,
		unsigned short func, unsigned short offset)
{
	return (uint16_t)(pciConfigReadDword(bus, slot, func, offset) & 0xFFFF);
}

uint32_t PCIDriverManager::pciConfigReadDword (unsigned short bus, unsigned short slot,
		unsigned short func, unsigned short offset)
{
	unsigned long address;
	unsigned long lbus = (unsigned long)bus;
	unsigned long lslot = (unsigned long)slot;
	unsigned long lfunc = (unsigned long)func;

	/* create configuration address as per Figure 1 */
	address = (unsigned long)((lbus << 16) | (lslot << 11) |
			(lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

	/* write out the address */
	outl (0xCF8, address);
	/* read in the data */
	return inl (0xCFC) >> ((offset & 2) * 8);
}



void PCIDriverManager::pciConfigWriteWord (unsigned short bus, unsigned short slot,
                                  unsigned short func, unsigned short offset, uint16_t value)
{
// stay broken for now :)
}

void PCIDriverManager::pciConfigWriteDword (unsigned short bus, unsigned short slot,
                                  unsigned short func, unsigned short offset, uint32_t value)
{
	unsigned long address;
	unsigned long lbus = (unsigned long)bus;
	unsigned long lslot = (unsigned long)slot;
	unsigned long lfunc = (unsigned long)func;

	/* create configuration address as per Figure 1 */
	address = (unsigned long)((lbus << 16) | (lslot << 11) |
			(lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

	/* write out the address */
	outl (0xCF8, address);
	/* read in the data */
	outl (0xCFC, value);
}
