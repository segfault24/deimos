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

#ifndef KERNEL_PCI_H
#define KERNEL_PCI_H

#include <stdint.h>

#define CFG_ADDRESS 0xCF8
#define CFG_DATA 0xCFC

// common configuration space header registers
#define CFG_W_VENDORID		0x00
#define CFG_W_DEVICEID		0x02
#define CFG_W_COMMAND		0x04
#define CFG_W_STATUS		0x06
#define CFG_B_REVISION		0x08
#define CFG_B_PROGIF		0x09
#define CFG_B_SUBCLASSID	0x0A
#define CFG_B_CLASSID		0x0B
#define CFG_B_CACHELINESIZE	0x0C
#define CFG_B_LATENCYTIMER	0x0D
#define CFG_B_HEADERTYPE	0x0E
#define CFG_B_BIST			0x0F

// headertype 00h registers
#define CFG0_L_BAR0			0x10
#define CFG0_L_BAR1			0x14
#define CFG0_L_BAR2			0x18
#define CFG0_L_BAR3			0x1C
#define CFG0_L_BAR4			0x20
#define CFG0_L_BAR5			0x24
#define CFG0_L_CARDBUSCIS	0x28
#define CFG0_W_SUBVENDORID	0x2C
#define CFG0_W_SUBID		0x2E
#define CFG0_L_EXPROM		0x30
#define CFG0_B_CAPPOINT		0x34
#define CFG0_B_INTLINE		0x3C
#define CFG0_B_INTPIN		0x3D
#define CFG0_B_MINGRANT		0x3E
#define CFG0_B_MAXLATENCY	0x3F

typedef struct _pci_dev_t {
	uint8_t bus;
	uint8_t dev;
	uint8_t func;
	
	uint16_t vendorid;
	uint16_t deviceid;
	uint8_t class;
	uint8_t subclass;
	
	uint8_t headertype;
	uint8_t intline;
	
	struct _pci_dev_t* next;
} pci_dev_t;

// read/write functions for PCI configuration space
uint32_t pci_readl(pci_dev_t* pdev, uint8_t offset);
uint16_t pci_readw(pci_dev_t* pdev, uint8_t offset);
uint8_t pci_readb(pci_dev_t* pdev, uint8_t offset);
void pci_writel(pci_dev_t* pdev, uint8_t offset, uint32_t value);
void pci_writew(pci_dev_t* pdev, uint8_t offset, uint16_t value);
void pci_writeb(pci_dev_t* pdev, uint8_t offset, uint8_t value);

void pci_init();
void pci_dump();
pci_dev_t* pci_search(uint16_t vendorid, uint16_t deviceid);

#endif
