/*
 *  Copyright (C) 2015  Austin Selick-Bottos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <kernel/stdio.h>
#include <i386/ioasm.h>
#include <kernel/kalloc.h>
#include <kernel/pci.h>

static pci_dev_t* pci_dev_list = 0;

uint32_t pci_config_read_long(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset)
{
	uint32_t address =
		((uint32_t)0x80000000) |
		(((uint32_t)bus)<<16 & 0x00FF0000) |
		(((uint32_t)dev)<<11 & 0x0000F800) |
		(((uint32_t)func)<<8 & 0x00000700) |
		(((uint32_t)offset) &  0x000000FC);
	outl(PCI_CONFIG_ADDRESS, address);
	return inl(PCI_CONFIG_DATA);
}

uint16_t pci_config_read_word(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset)
{
	uint32_t l = pci_config_read_long(bus, dev, func, offset);
	return (uint16_t)((l >> ((offset & 2) * 8)) & 0xFFFF);
}

uint8_t pci_config_read_byte(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset)
{
	uint32_t l = pci_config_read_long(bus, dev, func, offset);
	return (uint8_t)((l >> ((offset & 3) * 8)) & 0xFF);
}

static void add_pci_dev(uint8_t bus, uint8_t dev, uint8_t func)
{
	pci_dev_t* p;
	pci_dev_t* t;
	
	p = kmalloc(sizeof(pci_dev_t));
	if(!p)
		return;
	p->next = 0;
	p->bus = bus;
	p->dev = dev;
	p->func = func;
	p->vendorid = pci_config_read_word(bus, dev, func, PCI_CONFIG_W_VENDORID);
	p->deviceid = pci_config_read_word(bus, dev, func, PCI_CONFIG_W_DEVICEID);
	p->class = pci_config_read_byte(bus, dev, func, PCI_CONFIG_B_CLASSID);
	p->subclass = pci_config_read_byte(bus, dev, func, PCI_CONFIG_B_SUBCLASSID);
	
	if(!pci_dev_list)
	{
		pci_dev_list = p;
		return;
	}
	
	t = pci_dev_list;
	while(t->next)
		t = t->next;
	t->next = p;
}

void pci_init()
{
	uint16_t vendorid;
	uint8_t headertype;
	int bus, dev, func;
	
	for(bus=0; bus<256; bus++)
	{
		for(dev=0; dev<32; dev++)
		{
			func = 0;
			vendorid = pci_config_read_word(bus, dev, func, PCI_CONFIG_W_VENDORID);
			if(vendorid == 0xFFFF)
				continue;
			add_pci_dev(bus, dev, func);
			
			// check for multifunction
			headertype = pci_config_read_byte(bus, dev, func, PCI_CONFIG_B_HEADERTYPE);
			if(headertype & 0x80)
			{
				// check the rest of the functions
				for(func=1; func<8; func++)
				{
					vendorid = pci_config_read_word(bus, dev, func, PCI_CONFIG_W_VENDORID);
					if(vendorid == 0xFFFF)
						continue;
					add_pci_dev(bus, dev, func);
				}
			}
		}
	}
	
	pci_dump();
}

void pci_dump()
{
	pci_dev_t* p = pci_dev_list;
	while(p)
	{
		printf("%x:%x:%x | vend:%x dev:%x | class:%x subclass:%x\n",
			p->bus, p->dev, p->func,
			p->vendorid, p->deviceid,
			p->class, p->subclass);
		p = p->next;
	}
}

pci_dev_t* pci_search(uint16_t vendorid, uint16_t deviceid)
{
	pci_dev_t* p = pci_dev_list;
	while(p)
	{
		if(p->vendorid == vendorid && p->deviceid == deviceid)
			return p;
		p = p->next;
	}
	return 0;
}
