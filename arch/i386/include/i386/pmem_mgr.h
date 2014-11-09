#ifndef I386_PMEM_MGR_H
#define I386_PMEM_MGR_H

#include <stdint.h>

#define PMEM_BLOCK_SIZE  4096
// max possible blocks in a 4GiB system
#define PMEM_MAX_BLOCKS  1048576
#define PMEM_MAX_ENTRIES PMEM_MAX_BLOCKS/32

typedef uint32_t phys_addr;
typedef uint32_t virt_addr;

void pmem_mgr_init();
void pmem_mgr_free_region(phys_addr start, phys_addr end);
void pmem_mgr_reserve_region(phys_addr start, phys_addr end);

uint32_t pmem_total_blocks();
uint32_t pmem_free_blocks();
uint32_t pmem_used_blocks();

phys_addr pmem_mgr_alloc();
// freeing addresses not allocated produces undefined behavior
void pmem_mgr_free(phys_addr addr);

#endif
