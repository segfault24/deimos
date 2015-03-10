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

#ifndef KERNEL_DISK_H
#define KERNEL_DISK_H

#include <stddef.h>

typedef struct _lba_t {
	unsigned int high;
	unsigned int low;
} lba_t;
typedef lba_t blkcnt_t;

typedef struct _drive_t {
	// for use by the low level driver only
	void* priv;
	
	// low level driver must set these up
	size_t block_size;
	blkcnt_t num_blocks;
	
	// assigned by volume manager
	// TODO: unique drive identifier
	unsigned int drive_id;
	
	// int ata_read(drive_t* d, lba_t start_block, blkcnt_t num_blocks, char* dest)
	int(*readcallback)(struct _drive_t*, lba_t, blkcnt_t, char*);
	// int ata_write(drive_t* d, lba_t start_block, blkcnt_t num_blocks, char* src)
	int(*writecallback)(struct _drive_t*, lba_t, blkcnt_t, char*);
	
	// volume manager sets these up
	struct _volume_t* child_volumes;
	struct _drive_t* next;
} drive_t;

typedef struct _volume_t {
	int mounted;
	
	size_t block_size;
	lba_t offset_lba;
	blkcnt_t num_blocks;
	
	// assigned by volume manager
	unsigned int volume_id;
	
	// volume manager sets these up
	struct _drive_t* parent_drive;
	struct _volume_t* next;
} volume_t;

// functions for disk drivers
int register_drive(drive_t* d);
int remove_drive(drive_t* d);

// functions for filesystems
int read_volume(volume_t* v, lba_t relative_lba, blkcnt_t num_blocks, char* dest);
int write_volume(volume_t* v, lba_t relative_lba, blkcnt_t num_blocks, char* src);

// other
void disk_print_info();

#endif
