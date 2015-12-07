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

#ifndef I386_FRAME_H
#define I386_FRAME_H

#include <stddef.h>
#include <kernel/multiboot.h>
#include <i386/paging.h>

// architecture specified frame size
#define FRAME_SIZE  4096
// max possible frames in a 4GiB system
#define MAX_FRAMES  1024*1024

// we parse the multiboot memory map here
// this establishes our available physical memory
void frame_init(multiboot_info_t* mbt);

// allows systems to request specific physical pages
// returns the input if successful, 0 otherwise
phys_addr request_frame(phys_addr p);

// allocates a physical frame
// returns 0 if there are none available
phys_addr alloc_frame();

// freeing addresses not allocated produces undefined behavior
void free_frame(phys_addr addr);

// for getting the status of the physical memory
size_t num_total_frames();
size_t num_free_frames();
size_t num_used_frames();

#endif
