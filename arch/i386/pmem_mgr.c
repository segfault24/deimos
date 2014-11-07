#include <stddef.h>
#include <stdint.h>
#include <i386/pmem_mgr.h>

// 0: block free
// 1: block used
static uint32_t pmem_bitmap[PMEM_MAX_ENTRIES];
static uint32_t pmem_num_total_blocks;
static uint32_t pmem_num_free_blocks;
static uint32_t pmem_num_used_blocks;

static uint8_t test_block(uint32_t b)
{
	if((pmem_bitmap[b/32] & (1 << (b%32))) == 0)
	{
		return 0;
	} else {
		return 1;
	}
}

// mark the block if it isnt already
// update the block counts if necessary
static void mark_block(uint32_t b)
{
	if(!test_block(b))
	{
		pmem_bitmap[b/32] |= (1 << (b%32));
		pmem_num_used_blocks++;
		pmem_num_free_blocks--;
	}
}

// free the block if it isnt already
// update the block counts if necessary
// note: we can't free the first block
static void clear_block(uint32_t b)
{
	if(b!=0 && test_block(b))
	{
		pmem_bitmap[b/32] &= ~(1 << (b%32));
		pmem_num_used_blocks--;
		pmem_num_free_blocks++;
	}
}

uint32_t pmem_total_blocks()
{
	return pmem_num_total_blocks;
}

uint32_t pmem_free_blocks()
{
	return pmem_num_free_blocks;
}

uint32_t pmem_used_blocks()
{
	return pmem_num_used_blocks;
}

void pmem_mgr_init(uint32_t mem_size_in_blocks)
{
	size_t i;

	// round the number of blocks to a multiple of 32
	// setup our global block counters
	if(mem_size_in_blocks>PMEM_MAX_BLOCKS)
		mem_size_in_blocks = PMEM_MAX_BLOCKS;
	pmem_num_total_blocks = mem_size_in_blocks - mem_size_in_blocks%32;
	pmem_num_used_blocks = 0;
	pmem_num_free_blocks = pmem_num_total_blocks;
	// clear the bitmap (even if the mem doesnt exist)
	// TODO: should all mem be marked as used by default?
	for(i=0; i<PMEM_MAX_ENTRIES; i++)
	{
		pmem_bitmap[i] = 0;
	}
	// mark the first block as used
	mark_block(0);
}

void pmem_mgr_reserve_region(phys_addr start, phys_addr end)
{
	uint32_t i;
	uint32_t s_block = start/PMEM_BLOCK_SIZE;
	uint32_t e_block = end/PMEM_BLOCK_SIZE;

	for(i=s_block; i<=e_block && i<pmem_num_total_blocks; i++)
	{
		mark_block(i);
	}
}

void pmem_mgr_free_region(phys_addr start, phys_addr end)
{
	uint32_t i;
	uint32_t s_block = start/PMEM_BLOCK_SIZE;
	uint32_t e_block = end/PMEM_BLOCK_SIZE;

	for(i=s_block; i<=e_block && i<pmem_num_total_blocks; i++)
	{
		clear_block(i);
	}
}

phys_addr pmem_mgr_alloc()
{
	uint32_t i, j, block;

	// search the bitmap for a free block
	// i dont like nesting so much, but it's fairly simple
	for(i=0; i<pmem_num_total_blocks/32; i++)
	{
		if(pmem_bitmap[i] != 0xFFFFFFFF)
		{
			// extract which bit
			for(j=0; j<32; j++)
			{
				block = i*32+j;
				if(!test_block(block))
				{
					// mark the block as used in the map
					mark_block(block);
					// return the address
					return (phys_addr)(block*PMEM_BLOCK_SIZE);
				}
			}
		}
	}
	return 0;
}

void pmem_mgr_free(phys_addr addr)
{
	uint32_t block = addr/PMEM_BLOCK_SIZE;
	if(block<pmem_num_total_blocks)
	{
		clear_block(block);
	}
}
