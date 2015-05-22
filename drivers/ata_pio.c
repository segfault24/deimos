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

#include <stddef.h>
#include <stdint.h>
#include <i386/asm_util.h>
#include <kernel/stdio.h>
#include <kernel/kalloc.h>
#include <kernel/error.h>
#include <kernel/disk.h>

// bus base port addresses
#define BUS0 0x01F0
#define BUS1 0x0170
#define BUS2 0x01E8
#define BUS3 0x0168

// control/alternate status registers
#define BUS0_CTRL 0x03F6
#define BUS1_CTRL 0x0376
#define BUS2_CTRL 0x03E6
#define BUS3_CTRL 0x0366
#define BUS0_STATUS BUS0_CTRL
#define BUS1_STATUS BUS1_CTRL
#define BUS2_STATUS BUS2_CTRL
#define BUS3_STATUS BUS3_CTRL

// ATA port address offsets
#define DATA			0
#define FEATURES		1
#define ERRORINFO		1
#define SECTORCOUNT		2
#define SECTORNUMBER	3
#define CYLINDERLOW		4
#define CYLINDERHIGH	5
#define LBALOW			3
#define LBAMID			4
#define LBAHIGH			5
#define DRIVESELECT		6
#define COMMAND			7
#define STATUS			7

// status bits
#define ERR 0x01
#define DRQ 0x08
#define SRV 0x10
#define DF  0x20
#define RDY 0x40
#define BSY 0x80

// command bytes
#define IDENTIFY 0xEC
#define READ_SECTORS_EXT 0x24

// identify information (words)
#define I_SERIAL		10		// 10 words
#define I_FIRMWARE		23		// 4
#define I_MODEL			27		// 10
#define I_LBA28SECTORS	60		// 2
#define I_LBA48SECTORS	100		// 4
// TODO: multiword? DMA and UDMA modes

// for selecting master/slave & chs/lba
#define MASTERCHS   0xA0
#define SLAVECHS    0xB0
#define MASTERLBA28 0xE0
#define SLAVELBA28  0xF0
#define MASTERLBA48 0x40
#define SLAVELBA48  0x50

///////////////////////////////////
// for our internal implementation
#define CHS_MODE 0x1
#define LBA28_MODE 0x2
#define LBA48_MODE 0x3

typedef struct _ata_priv_t {
	int mode;
	uint16_t bus;
	uint16_t drive;
	uint16_t status_reg;
} ata_priv_t;

static int started = 0;
static drive_t* drives[8];

static int ata_lba48_read(drive_t* d, lba_t start_block, blkcnt_t num_blocks, char* dest)
{
	// TODO: type sizes are a concern in this function
	// TODO: eliminate this max sector limit by looping
	if(num_blocks.high || num_blocks.low>65536)
		return 1;
	
	// convert logical sectors to physical
	// TODO: this is a mess
	uint32_t sectors = (d->block_size/512) * num_blocks.low;
	// check the special case
	if(sectors == 65536)
		sectors = 0;
	
	// extract the drive's bus/drive/status registers
	uint16_t bus = ((ata_priv_t*)(d->priv))->bus;
	uint16_t drive = ((ata_priv_t*)(d->priv))->drive;
	uint16_t status_reg = ((ata_priv_t*)(d->priv))->status_reg;
	
	// setup and execute the command
	outb(bus + DRIVESELECT, drive);
	outb(bus + SECTORCOUNT, (char)(sectors>>8));
	outb(bus + LBALOW, (char)(start_block.low>>24));
	outb(bus + LBAMID, (char)(start_block.high));
	outb(bus + LBAHIGH, (char)(start_block.high>>8));
	outb(bus + SECTORCOUNT, (char)(sectors));
	outb(bus + LBALOW, (char)(start_block.low));
	outb(bus + LBAMID, (char)(start_block.low>>8));
	outb(bus + LBAHIGH, (char)(start_block.low>>16));
	outb(bus + COMMAND, READ_SECTORS_EXT);
	
	size_t s, i;
	for(s=0; s<sectors; s++)
	{
		// 400ns delay
		inb(status_reg);
		inb(status_reg);
		inb(status_reg);
		inb(status_reg);
		// wait for it to be ready
		while((inb(status_reg) & BSY) | !(inb(status_reg) & DRQ)); // TODO: timeout
		for(i=0; i<256; i++)
		{
			*((uint16_t*)dest) = inw(bus + DATA);
			dest += 2;
		}
	}
	return 0;
}

static int ata_lba28_read(drive_t* d, lba_t start_block, blkcnt_t num_blocks, char* dest)
{
	d++;start_block.low++;num_blocks.low++;dest++;
	return 1;
}

static int ata_chs_read(drive_t* d, lba_t start_block, blkcnt_t num_blocks, char* dest)
{
	d++;start_block.low++;num_blocks.low++;dest++;
	return 1;
}

static int ata_lba48_write(drive_t* d, lba_t start_block, blkcnt_t num_blocks, char* src)
{
	d++;start_block.low++;num_blocks.low++;src++;
	return 1;
}

static int ata_lba28_write(drive_t* d, lba_t start_block, blkcnt_t num_blocks, char* src)
{
	d++;start_block.low++;num_blocks.low++;src++;
	return 1;
}

static int ata_chs_write(drive_t* d, lba_t start_block, blkcnt_t num_blocks, char* src)
{
	d++;start_block.low++;num_blocks.low++;src++;
	return 1;
}

// returns pointer to a new drive_t struct or 0
static drive_t* identify(unsigned int bus, unsigned int drive, unsigned int status_reg)
{
	outb(bus + DRIVESELECT, drive);
	outb(bus + SECTORCOUNT, 0);
	outb(bus + LBALOW, 0);
	outb(bus + LBAMID, 0);
	outb(bus + LBAHIGH, 0);
	outb(bus + COMMAND, IDENTIFY); // send the command
	
	// stall for time to allow the drive to push its status
	inb(status_reg);
	inb(status_reg);
	inb(status_reg);
	inb(status_reg);
	
	// see if device actually exists
	if(inb(status_reg) == 0)
		return 0; 
	
	// wait for it to be ready
	while(inb(status_reg) & BSY); // TOOD: timeout
	
	// check if it's not an ATA drive
	if((inb(status_reg) & ERR) || inb(bus + LBAMID) != 0 || inb(bus + LBAHIGH) != 0)
		return 0;
	
	// read the drive's IDENTIFY info
	uint16_t* info = kmalloc(2*256);
	if(!info)
		return 0;
	size_t i;
	for(i=0; i<256; i++)
		info[i] = inw(bus + DATA);
	
	// serial is spec mandatory, if it's zero
	// something went wrong, discard the drive
	if(info[I_SERIAL] == 0)
	{
		kfree(info);
		return 0;
	}
	
	// setup the new struct
	drive_t* d = kmalloc(sizeof(drive_t));
	ata_priv_t* p = kmalloc(sizeof(ata_priv_t));
	if(!d || !p)
	{
		kfree(d);
		kfree(p);
		kfree(info);
		kwarn("ata_pio: could not allocate memory for drive struct");
		return 0;
	}
	
	// TODO: populate the drive_t and ata_priv_t
	if(info[83] & 0x0400) // LBA48
	{
		p->mode = LBA48_MODE;
		p->bus = bus;
		if(drive & 0x10)
			p->drive = SLAVELBA48;
		else
			p->drive = MASTERLBA48;
		p->status_reg = status_reg;
		
		d->num_blocks.high =
			(((uint32_t)info[I_LBA48SECTORS + 3]) << 16) |
			((uint32_t)info[I_LBA48SECTORS + 2]);
		d->num_blocks.low =
			(((uint32_t)info[I_LBA48SECTORS + 1]) << 16) |
			((uint32_t)info[I_LBA48SECTORS]);
		d->readcallback = &ata_lba48_read;
		d->writecallback = &ata_lba48_write;
	}
	else if(0) // LBA28
	{
		// TODO: how to check for LBA28
		p->mode = LBA28_MODE;
		d->num_blocks.high = 0;
		d->num_blocks.low =
			(((uint32_t)info[I_LBA28SECTORS + 1]) << 16) |
			((uint32_t)info[I_LBA28SECTORS]);
		d->readcallback = &ata_lba28_read;
		d->writecallback = &ata_lba28_write;
	}
	else // default to CHS
	{
		// TODO: this
		p->mode = CHS_MODE;
		d->num_blocks.high = 0;
		d->num_blocks.low = 0;
		d->readcallback = &ata_chs_read;
		d->writecallback = &ata_chs_write;
	}
	d->block_size = 512;
	d->child_volumes = 0;
	d->next = 0;
	d->priv = p;
	
	kfree(info);
	return d;
}

int ata_module_init()
{
	if(started)
	{
		kwarn("ata_pio: module already running");
		return 1;
	}
	
	drives[0] = identify(BUS0, MASTERCHS, BUS0_STATUS);
	drives[1] = identify(BUS0, SLAVECHS, BUS0_STATUS);
	drives[2] = identify(BUS1, MASTERCHS, BUS1_STATUS);
	drives[3] = identify(BUS1, SLAVECHS, BUS1_STATUS);
	drives[4] = 0;
	drives[5] = 0;
	drives[6] = 0;
	drives[7] = 0;
	
	size_t i;
	int good = 0;
	for(i=0; i<8; i++)
	{
		if(drives[i])
		{
			if(register_drive(drives[i]))
			{
				kfree(drives[i]);
				kwarn("ata_pio: could not register drive");
			}
			else
			{
				good = 1;
			}
		}
	}
	
	// if we didn't register anything, fail
	if(!good)
	{
		kwarn("ata_pio: module not started, no drives registered");
		return 1;
	}
	
	started = 1;
	return 0;
}

int ata_module_kill()
{
	if(!started)
	{
		kwarn("ata_pio: module wasn't running");
		return 1;
	}
	
	size_t i;
	int fail = 0;
	for(i=0; i<8; i++)
	{
		if(drives[i])
		{
			if(remove_drive(drives[i]))
			{
				fail = 1;
				kwarn("ata_pio: could not remove drive");
			}
			else
			{
				kfree(drives[i]->priv);
				kfree(drives[i]);
				drives[i] = 0;
			}
		}
	}
	// if there's still a drive registered, continue running
	if(fail)
	{
		kwarn("ata_pio: module still running, a drive is open");
		return 1;
	}
	
	started = 0;
	return 0;
}
