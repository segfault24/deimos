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
