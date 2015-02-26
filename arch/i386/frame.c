#include <stddef.h>
#include <stdint.h>
#include <kernel/multiboot.h>
#include <i386/boot.h>
#include <i386/paging.h>
#include <i386/frame.h>

// We keep track of memory that has been allcoated using
// a bitmap. each bit corresponds to a frame and its status
// is indicated by: 0=free, 1=used
// We assume nothing for actual available physical memory
// except that there's a max of 4GiB and then initialize
// by reading from the multiboot info struct

static uint32_t frame_bitmap[MAX_FRAMES/sizeof(uint32_t)];
static size_t _num_total_frames = 0;
static size_t _num_free_frames = 0;
static size_t _num_used_frames = 0;

static void set_bit(size_t b)
{
	frame_bitmap[b/32] |= (1 << (b%32));
}

static void clr_bit(size_t b)
{
	frame_bitmap[b/32] &= ~(1 << (b%32));
}

static int get_bit(size_t b)
{
	return (frame_bitmap[b/32] & (1 << (b%32))) ? 1 : 0;
}

static void free_region(phys_addr addr, size_t size)
{
	size_t i;

	for(i=addr/FRAME_SIZE; i<=(addr+size-1)/FRAME_SIZE; i++)
	{
		// if it isnt already freed (and not the zero block)
		if(i && get_bit(i))
		{
			clr_bit(i);
			_num_free_frames++;
			_num_total_frames++;
		}
	}
}

static void reserve_region(phys_addr addr, size_t size)
{
	size_t i;

	for(i=addr/FRAME_SIZE; i<=(addr+size-1)/FRAME_SIZE; i++)
	{
		// if it isnt already used
		if(get_bit(i))
		{
			set_bit(i);
			_num_free_frames--;
			_num_total_frames--;
		}
	}
}

/***** public functions *****/

void frame_init(multiboot_info_t* mbt)
{
	size_t i;
	// clear the bitmap, mark all frames as used by default
	for(i=0; i<MAX_FRAMES/sizeof(uint32_t); i++)
		frame_bitmap[i] = 0xFFFFFFFF;
	
	// get the multiboot memory map and its length
	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mbt->mmap_addr;
	multiboot_uint32_t end = mbt->mmap_addr + mbt->mmap_length;

	// parse the memory map and configure the physical memory manager
	while((multiboot_uint32_t)mmap < end)
	{
		// mark regions labeled "available" as free for use
		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
			free_region(mmap->addr, mmap->addr+mmap->len-1);
		// move to next entry
		mmap = (multiboot_memory_map_t*)
			((unsigned int)mmap + mmap->size + sizeof(unsigned int));
	}
	
	// make sure the kernel's physical location is reserved
	reserve_region(KERNEL_PMA, KERNEL_SIZE);
}

phys_addr alloc_frame()
{
	size_t i, j, b;

	// search the bitmap for a free block
	// i dont like nesting so much, but it's fairly simple
	for(i=0; i<MAX_FRAMES/sizeof(uint32_t); i++)
	{
		if(frame_bitmap[i] != 0xFFFFFFFF)
		{
			// extract which bit
			for(j=0; j<32; j++)
			{
				b = i*32 + j;
				if(!get_bit(b))
				{
					// mark the block as used in the map
					frame_bitmap[b/32] |= (1 << (b%32));
					_num_used_frames++;
					_num_free_frames--;
					// return the address
					return (phys_addr)(b*FRAME_SIZE);
				}
			}
		}
	}
	return 0;
}

void free_frame(phys_addr addr)
{
	size_t b = addr/FRAME_SIZE;
	// the block is in use and not the zero block
	if(b && get_bit(b))
	{
		clr_bit(b);
		_num_used_frames--;
		_num_free_frames++;
	}
}

size_t num_total_frames(){return _num_total_frames;}
size_t num_free_frames(){return _num_free_frames;}
size_t num_used_frames(){return _num_used_frames;}
