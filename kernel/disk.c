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
#include <kernel/string.h>
#include <kernel/kalloc.h>
#include <kernel/error.h>
#include <kernel/disk.h>

typedef struct _partition_entry_t {
	uint8_t boot_indicator;
	uint8_t starting_head;
	uint16_t starting_sector_cylinder;
	uint8_t system_id;
	uint8_t ending_head;
	uint16_t ending_sector_cylinder;
	uint32_t relative_sector;
	uint32_t total_sectors;
} __attribute__((packed)) partition_entry_t;

typedef struct _mbr_t {
	uint8_t bootstrap[436];
	uint8_t disk_id[10];
	partition_entry_t partitions[4];
	uint16_t signature;
} __attribute__((packed)) mbr_t;

static drive_t* drive_list = 0;
static unsigned int next_drive_id = 0;

// reads MBR sets up the drive's child volumes
// TODO: sanity check MBR partition entries
static int parse_mbr(drive_t* d)
{
	unsigned char buf[d->block_size];
	size_t i;
	
	// read the first sector into memory
	lba_t l; l.high=0; l.low=0;
	blkcnt_t b; b.high=0; b.low=1;
	if(d->readcallback(d, l, b, (char*)&buf))
	{
		kwarn("dvm: could not read disk");
		return 1;
	}
	mbr_t* mbr = (mbr_t*)&buf;
	
	// check the signature first
	if(mbr->signature != 0xAA55)
	{
		kwarn("dvm: invalid MBR signature");
		return 1;
	}
	
	// assign it the next drive id
	d->drive_id = next_drive_id++;
	
	// read the partition table entries
	for(i=0; i<4; i++)
	{
		// check if the entry is actually in use
		if(mbr->partitions[i].system_id == 0)
			continue;
		
		volume_t* v = kmalloc(sizeof(volume_t));
		if(!v)
		{
			kwarn("dvm: could not allocate memory for volume struct");
			return 1;
			// note: the calling function should free any
			//       previously allocated volume structs
		}
		memset(v, 0, sizeof(volume_t));
		
		// TODO: convert physical to logical blocks?
		v->volume_id = i;
		v->parent_drive = d;
		v->block_size = d->block_size;
		v->offset_lba.high = 0;
		v->offset_lba.low = mbr->partitions[i].relative_sector;
		v->num_blocks.high = 0;
		v->num_blocks.low = mbr->partitions[i].total_sectors;
		
		// add it to the drive's volume list
		if(d->child_volumes == 0)
		{
			d->child_volumes = v;
		}
		else
		{
			volume_t* t = d->child_volumes;
			while(t->next)
				t = t->next;
			t->next = v;
		}
	}
	return 0;
}

// functions for disk drivers
int register_drive(drive_t* d)
{
	// parse the drive's MBR
	if(parse_mbr(d))
	{
		kwarn("dvm: could not parse MBR");
		// free any children that might have been created
		volume_t* v = d->child_volumes;
		while(v)
		{
			volume_t* t = v->next;
			kfree(v);
			v = t;
		}
		return 1;
	}
	
	// add drive_t to drive list
	d->next = 0;
	if(!drive_list)
	{
		drive_list = d;
		return 0;
	}
	
	drive_t* t = drive_list;
	while(t->next)
		t = t->next;
	t->next = d;
	return 0;
}

int remove_drive(drive_t* d)
{
	// free the drive's volume_t's
	volume_t* v = d->child_volumes;
	while(v)
	{
		if(v->mounted)
		{
			kwarn("dvm: volume still mounted");
			return 1;
		}
		volume_t* t = v->next;
		kfree(v);
		v = t;
	}
	
	// remove the drive_t from the drive list
	if(!drive_list)
		return 1;
	
	if(d == drive_list)
	{
		drive_list = d->next;
		d->next = 0;
		return 0;
	}
	
	drive_t* t = drive_list;
	while(t->next)
	{
		if(t->next == d)
		{
			t->next = t->next->next;
			d->next = 0;
			return 0;
		}
		t = t->next;
	}
	return 1;
}

// functions for filesystems
int read_volume(volume_t* v, lba_t relative_lba, blkcnt_t num_blocks, char* dest)
{
	v++;relative_lba.low++;num_blocks.low++;dest++;//dummy
	return 1;
	// TODO: add drive and partition bounds checks
	/*return (v->parent_drive->readcallback)(
				v->parent_drive,
				v->start_offset + start_block,
				num_blocks,
				dest
			);*/
}

int write_volume(volume_t* v, lba_t relative_lba, blkcnt_t num_blocks, char* src)
{
	v++;relative_lba.low++;num_blocks.low++;src++;//dummy
	return 1;
	// TODO: add drive and parition bounds checks
	/*return (v->parent_drive->writecallback)(
				v->parent_drive,
				v->start_offset + start_block,
				num_blocks,
				src
			);*/
}

void disk_print_info()
{
	drive_t* d = drive_list;
	while(d)
	{
		printf("hdd%u\tsize:%x%x\n", d->drive_id, d->num_blocks.high, d->num_blocks.low);
		volume_t* v = d->child_volumes;
		while(v)
		{
			printf("\thdd%uvol%u\toffset:%x%x\tsize:%x%x\n",
				v->parent_drive->drive_id, v->volume_id,
				v->offset_lba.high, v->offset_lba.low,
				v->num_blocks.high, v->num_blocks.low
			);
			v = v->next;
		}
		d = d->next;
	}
}
