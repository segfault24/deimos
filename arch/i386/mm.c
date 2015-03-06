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

#include <stdint.h>

#include <kernel/multiboot.h>
#include <kernel/stdlib.h>
#include <kernel/stdio.h>
#include <kernel/error.h>
#include <kernel/kalloc.h>
#include <kernel/mm.h>

#include <i386/boot.h>
#include <i386/frame.h>
#include <i386/paging.h>

// provided by the linker script
extern void* KERNEL_END;

void mm_init(multiboot_info_t* mbt)
{
	// check if the memory map is present
	if(!(mbt->flags & MULTIBOOT_INFO_MEM_MAP))
		kpanic("no memory map available in multiboot info structure");
	
	// initialize the frame allocator
	frame_init(mbt);
	
	// setup paging (we replace the bootstrapped setup)
	paging_init();
	
	// setup the kernel heap
	kheap_init(&KERNEL_END, KERNEL_VMA+KERNEL_PMA+KERNEL_SIZE-(int)&KERNEL_END);
}

void memory_print_info()
{
	printf("physical memory\n");
	printf("\ttotal: %d KiB\n", 4*num_total_frames());
	printf("\t free: %d KiB\n", 4*num_free_frames());
	printf("\t used: %d KiB\n", 4*num_used_frames());
}
