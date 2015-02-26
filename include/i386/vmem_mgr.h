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

#ifndef I386_VMEM_MGR_H
#define I386_VMEM_MGR_H

#include <stdint.h>
#include <i386/paging.h>

// virtual page size == physical block size
#define PAGE_SIZE FRAME_SIZE

// these functions interface with the physical memory
// manager to allocate a physical block for the page
uint8_t vmem_mgr_alloc_page(pt_entry* e);
void vmem_mgr_free_page(pt_entry* e);

// retrieve the current directory
page_directory_t* vmem_mgr_get_directory();
// switch in a new directory
void vmem_mgr_switch_directory(page_directory_t* pd);
// flush an entry from the TLB
void vmem_mgr_flush_tlb_entry(virt_addr addr);

void vmem_mgr_map_page(phys_addr paddr, virt_addr vaddr);
void vmem_mgr_pf_handler();
void vmem_mgr_init();

#endif
