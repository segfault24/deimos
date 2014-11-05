#ifndef I386_PMEM_MGR_H
#define I386_PMEM_MGR_H

#include <stddef.h>
#include <stdint.h>

#define PMEM_BLOCK_SIZE  4096
// max possible blocks in a 4GiB system
#define PMEM_MAX_BLOCKS  1048576
#define PMEM_MAX_ENTRIES PMEM_MAX_BLOCKS/32

void pmem_mgr_init();

uint32_t pmem_total_blocks();
uint32_t pmem_free_blocks();
uint32_t pmem_used_blocks();

void pmem_mgr_reserve_region(void* start, void* end);
void pmem_mgr_free_region(void* start, void* end);

void* pmem_mgr_alloc();
void pmem_mgr_free(void* addr);

#endif
