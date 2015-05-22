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

#include <kernel/string.h>
#include <kernel/stdio.h>
#include <i386/asm_util.h>
#include <kernel/kalloc.h>
#include <kernel/pci.h>

static pci_dev_t* pci_dev_list = 0;

// helper function to format a pci configuration space access address
static uint32_t calc_pci_addr(pci_dev_t* pdev, uint8_t offset)
{
	return
		((uint32_t)0x80000000) |
		(((uint32_t)pdev->bus)<<16 & 0x00FF0000) |
		(((uint32_t)pdev->dev)<<11 & 0x0000F800) |
		(((uint32_t)pdev->func)<<8 & 0x00000700) |
		(((uint32_t)offset) &  0x000000FC);
}

// helper function for the main enumeration loop that creates
// a pci_dev_t for the given bus/dev/func (assumed valid)
// and adds it to the list
static void add_pci_dev(uint8_t bus, uint8_t dev, uint8_t func)
{
	pci_dev_t* p;
	pci_dev_t* t;
	
	p = kmalloc(sizeof(pci_dev_t));
	if(!p)
		return;
	memset(p, 0, sizeof(pci_dev_t));
	
	p->next = 0;
	p->bus = bus;
	p->dev = dev;
	p->func = func;
	p->vendorid = pci_readw(p, CFG_W_VENDORID);
	p->deviceid = pci_readw(p, CFG_W_DEVICEID);
	p->class = pci_readb(p, CFG_B_CLASSID);
	p->subclass = pci_readb(p, CFG_B_SUBCLASSID);
	p->headertype = pci_readb(p, CFG_B_HEADERTYPE);
	
	if(p->headertype == 0x00)
	{
		p->intline = pci_readb(p, CFG0_B_INTLINE);
	}
	
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

uint32_t pci_readl(pci_dev_t* pdev, uint8_t offset)
{
	outl(CFG_ADDRESS, calc_pci_addr(pdev, offset));
	return inl(CFG_DATA);
}

uint16_t pci_readw(pci_dev_t* pdev, uint8_t offset)
{
	return (uint16_t)((pci_readl(pdev, offset) >> ((offset & 2) * 8)) & 0xFFFF);
}

uint8_t pci_readb(pci_dev_t* pdev, uint8_t offset)
{
	return (uint8_t)((pci_readl(pdev, offset) >> ((offset & 3) * 8)) & 0xFF);
}

void pci_writel(pci_dev_t* pdev, uint8_t offset, uint32_t value)
{
	outl(CFG_ADDRESS, calc_pci_addr(pdev, offset));
	outl(CFG_DATA, value);
}

void pci_writew(pci_dev_t* pdev, uint8_t offset, uint16_t value)
{
	uint32_t t, s, mask;
	
	t = pci_readl(pdev, offset);
	mask = ~(((uint32_t)0xFFFF) << ((offset & 2) * 8));
	s = (uint32_t)value << ((offset & 2) * 8);
	t = (t&mask) | s;
	pci_writel(pdev, offset, t);
}

void pci_writeb(pci_dev_t* pdev, uint8_t offset, uint8_t value)
{
	uint32_t t, s, mask;
	
	t = pci_readl(pdev, offset);
	mask = ~(((uint32_t)0xFF) << ((offset & 3) * 8));
	s = (uint32_t)value << ((offset & 3) * 8);
	t = (t&mask) | s;
	pci_writel(pdev, offset, t);
}

void pci_init()
{
	uint16_t vendorid;
	uint8_t headertype;
	int bus, dev, func;
	pci_dev_t dummy;
	
	// populate the pci device list
	// brute force loop of all buses and devices
	for(bus=0; bus<256; bus++)
	{
		for(dev=0; dev<32; dev++)
		{
			func = 0;
			dummy.bus = bus; dummy.dev = dev; dummy.func = func;
			vendorid = pci_readw(&dummy, CFG_W_VENDORID);
			if(vendorid == 0xFFFF)
				continue;
			add_pci_dev(bus, dev, func);
			
			// check for multifunction
			headertype = pci_readb(&dummy, CFG_B_HEADERTYPE);
			if(headertype & 0x80)
			{
				// check the rest of the functions
				for(func=1; func<8; func++)
				{
					dummy.bus = bus; dummy.dev = dev; dummy.func = func;
					vendorid = pci_readw(&dummy, CFG_W_VENDORID);
					if(vendorid == 0xFFFF)
						continue;
					add_pci_dev(bus, dev, func);
				}
			}
		}
	}
}

void pci_print_info()
{
	pci_dev_t* p = pci_dev_list;
	while(p)
	{
		printf("bdf:%d,%d,%d | vend:%x dev:%x | class:%x subclass:%x\n",
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
