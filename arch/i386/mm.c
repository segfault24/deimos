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
#include <kernel/stdio.h>
#include <kernel/error.h>
#include <kernel/kalloc.h>
#include <kernel/mm.h>

#include <i386/boot.h>
#include <i386/frame.h>
#include <i386/paging.h>


void mm_init(multiboot_info_t* mbt)
{
	// check if the memory map is present
	if(!(mbt->flags & 0x20))
		kpanic("no memory map available in multiboot info structure");
	
	// initialize the frame allocator
	frame_init(mbt);
	
	// setup paging (we replace the bootstrapped setup)
	paging_init();
	
	// setup the kernel heap
	// TODO: these need to be virtual addresses, not physical...
	// TODO: we should probably update the vmem_mgr's static variables too...
	//kheap_init(&kheap_start, &kheap_end);
	
	// TESTING BELOW, DELETE WHEN DONE
	//while(1)
	//{
	//	tty_putv("alloc:", (uint32_t)kmalloc(1024*64-4), " ");
	//	kheap_available();
	//	tty_getchar();
	//}
	//kheap_available();
	//void* test1 = kmalloc(1024*512);tty_putv("alloc:", (uint32_t)test1, "\n");
	//kheap_available();
	//void* test2 = kmalloc(1024*512);tty_putv("alloc:", (uint32_t)test2, "\n");
	//kheap_available();
	//kfree(test1);tty_putv("free:", (uint32_t)test1, "\n");
	//kheap_available();
	//kfree(test2);tty_putv("free:", (uint32_t)test2, "\n");
	//kheap_available();
}

/////////////////////////////////////////////////////////////////////
//print_page_directory(vmem_mgr_get_directory());

/*void print_page_table(pd_entry pde, size_t pdi, page_table* pt)
{
	pt_entry pte;
	size_t i;

	for(i=0; i<PT_PAGES_PER_TABLE; i+=16)
	{
		pte = pt->entries[i];
		if(pt_entry_is_present(pte))
		{
			putv("PDE: ", (uint32_t)pde, "  ");
			putv("PTE: ", (uint32_t)pte, "  ");
			putv("VADDR: ", (uint32_t)(4096*(pdi*1024+i)), "  ");
			putv("PADDR: ", (uint32_t)(pte & 0xFFFFF000), "\n");
		}
		tty_getchar();
	}
}

void print_page_directory(page_directory* pd)
{
	pd_entry pde;
	page_table* pt;
	size_t i;

	for(i=0; i<PD_TABLES_PER_DIR; i++)
	{
		pde = pd->entries[i];
		if(pd_entry_is_present(pde))
		{
			pt = (page_table*)(pde & ~0xfff);
			print_page_table(pde, i, pt);
		}
	}
}*/