#ifndef I386_VMEM_MGR_H
#define I386_VMEM_MGR_H

#include <stddef.h>
#include <stdint.h>

// virtual page size == physical block size
#define VMEM_PAGE_SIZE PMEM_BLOCK_SIZE

// these functions interface with the physical memory
// manager to allocate a physical block for the page
uint8_t vmem_mgr_alloc_page(pt_entry* e);
void vmem_mgr_free_page(pt_entry* e);

// retrieve the current directory
inline page_directory* vmem_mgr_get_directory();
// switch in a new directory
inline uint8_t vmem_mgr_switch_directory(page_directory* pd);
// flush an entry from the TLB
void vmem_mgr_flush_tlb_entry(void* vaddr);

void vmem_mgr_map_page(void* paddr, void* vaddr);
void vmem_mgr_pf_handler();
void vmem_mgr_init();

#endif
