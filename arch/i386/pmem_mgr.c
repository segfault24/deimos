#include <stddef.h>
#include <stdint.h>
#include <i386/pmem_mgr.h>

// we keep track of memory that has been allcoated using
// a bitmap. each bit corresponds to a block and its status
// is indicated by:
// 0=free
// 1=used
// we assume nothing for actual available physical memory
// except that there's a max of 4GiB and later initialize
// by reading from the multiboot struct

static uint32_t pmem_bitmap[PMEM_MAX_ENTRIES];
static uint32_t pmem_num_total_blocks = 0;
static uint32_t pmem_num_free_blocks = 0;
static uint32_t pmem_num_used_blocks = 0;

static uint8_t test_block(uint32_t b)
{
	if((pmem_bitmap[b/32] & (1 << (b%32))) == 0)
		return 0;
	else
		return 1;
}

// mark the block if it isnt already
uint32_t pmem_total_blocks(){return pmem_num_total_blocks;}
uint32_t pmem_free_blocks(){return pmem_num_free_blocks;}
uint32_t pmem_used_blocks(){return pmem_num_used_blocks;}

void pmem_mgr_init()
{
	size_t i;
	// clear the bitmap, all mem marked as used by default
	for(i=0; i<PMEM_MAX_ENTRIES; i++)
		pmem_bitmap[i] = 0xFFFFFFFF;
}

void pmem_mgr_free_region(phys_addr start, phys_addr end)
{
	uint32_t i;
	uint32_t s_block = start/PMEM_BLOCK_SIZE;
	uint32_t e_block = end/PMEM_BLOCK_SIZE;

	for(i=s_block; i<=e_block; i++)
	{
		// if it isnt already freed (and not the zero block)
		if(i && test_block(i))
		{
			pmem_bitmap[i/32] &= ~(1 << (i%32));
			pmem_num_free_blocks++;
			pmem_num_total_blocks++;
		}
	}
}

void pmem_mgr_reserve_region(phys_addr start, phys_addr end)
{
	uint32_t i;
	uint32_t s_block = start/PMEM_BLOCK_SIZE;
	uint32_t e_block = end/PMEM_BLOCK_SIZE;

	for(i=s_block; i<=e_block; i++)
	{
		// if it isnt already used
		if(!test_block(i))
		{
			pmem_bitmap[i/32] |= (1 << (i%32));
			pmem_num_free_blocks--;
			pmem_num_total_blocks--;
		}
	}
}

phys_addr pmem_mgr_alloc()
{
	uint32_t i, j, block;

	// search the bitmap for a free block
	// i dont like nesting so much, but it's fairly simple
	for(i=0; i<PMEM_MAX_ENTRIES; i++)
	{
		if(pmem_bitmap[i] != 0xFFFFFFFF)
		{
			// extract which bit
			for(j=0; j<32; j++)
			{
				block = i*32 + j;
				if(!test_block(block))
				{
					// mark the block as used in the map
					pmem_bitmap[block/32] |= (1 << (block%32));
					pmem_num_used_blocks++;
					pmem_num_free_blocks--;
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
	// the block is in use and not the zero block
	if(test_block(block) && block)
	{
		pmem_bitmap[block/32] &= ~(1 << (block%32));
		pmem_num_used_blocks--;
		pmem_num_free_blocks++;
	}
}
