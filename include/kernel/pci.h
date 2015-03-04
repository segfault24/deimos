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

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

// common configuration space header registers
#define PCI_CONFIG_W_VENDORID		0x00
#define PCI_CONFIG_W_DEVICEID		0x02
#define PCI_CONFIG_W_COMMAND		0x04
#define PCI_CONFIG_W_STATUS			0x06
#define PCI_CONFIG_B_REVISION		0x08
#define PCI_CONFIG_B_PROGIF			0x09
#define PCI_CONFIG_B_SUBCLASSID		0x0A
#define PCI_CONFIG_B_CLASSID		0x0B
#define PCI_CONFIG_B_CACHELINESIZE	0x0C
#define PCI_CONFIG_B_LATENCYTIMER	0x0D
#define PCI_CONFIG_B_HEADERTYPE		0x0E
#define PCI_CONFIG_B_BIST			0x0F

// class codes
#define PCI_CLASS_PREDATED		0x00
#define PCI_CLASS_MASSSTORAGE	0x01
#define PCI_CLASS_NETWORK		0x02
#define PCI_CLASS_DISPLAY		0x03
#define PCI_CLASS_MULTIMEDIA	0x04
#define PCI_CLASS_MEMORY		0x05
#define PCI_CLASS_BRIDGE		0x06
#define PCI_CLASS_SIMPLECOMM	0x07
#define PCI_CLASS_BASESYSPERI	0x08
#define PCI_CLASS_INPUT			0x09
#define PCI_CLASS_DOCKING		0x0A
#define PCI_CLASS_PROCESSOR		0x0B
#define PCI_CLASS_SERIALBUS		0x0C
#define PCI_CLASS_WIRELESS		0x0D
#define PCI_CLASS_INTELLIGENTIO	0x0E
#define PCI_CLASS_SATCOMM		0x0F
#define PCI_CLASS_CRYPTO		0x10
#define PCI_CLASS_SIGPROC		0x11
#define PCI_CLASS_UNDEFINED		0xFF

//// subclass codes
#define PCI_SUBCLASS_OTHER		0x80
// 0x00 predated
#define PCI_SUBCLASS_NONVGA		0x00
#define PCI_SUBCLASS_VGA		0x01
// 0x01 mass storage
#define PCI_SUBCLASS_SCSI		0x00
#define PCI_SUBCLASS_IDE		0x01
#define PCI_SUBCLASS_FLOPPY		0x02
#define PCI_SUBCLASS_IPI		0x03
#define PCI_SUBCLASS_RAID		0x04
#define PCI_SUBCLASS_ATA		0x05
#define PCI_SUBCLASS_SATA		0x06
#define PCI_SUBCLASS_SAS		0x07
// 0x02 network

typedef struct _pci_dev_t {
	uint8_t bus;
	uint8_t dev;
	uint8_t func;
	
	uint16_t vendorid;
	uint16_t deviceid;
	uint8_t class;
	uint8_t subclass;
	
	struct _pci_dev_t* next;
} pci_dev_t;

uint32_t pci_config_read_long(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);
uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);
uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);

void pci_init();
void pci_dump();
pci_dev_t* pci_search(uint16_t vendorid, uint16_t deviceid);

#endif
