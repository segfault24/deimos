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

#include <stddef.h>
#include <stdint.h>

#include <i386/paging.h>
#include <kernel/stdio.h>
#include <kernel/error.h>
#include <kernel/string.h>
#include <kernel/kalloc.h>

#define HEAP_MAGIC 0xDEADBEEF

typedef struct _header_t {
	unsigned int magic;
	size_t size; // includes header
	int allocated;
	struct _header_t* prev;
	struct _header_t* next;
} header_t;

static header_t* kheap = 0;

// TODO: this. it'll eliminate duplicate logic
// splits the given block s bytes in
// returns pointer to the new block
// returns 0 if it couldn't
static header_t* split_block(header_t* h, size_t dist)
{
	header_t* new_h;
	
	// make sure the cut point places the new header within the existing block's data area
	if(dist > h->size - sizeof(header_t) || dist < sizeof(header_t))
		return 0;
	
	new_h = (void*)h + dist;
	new_h->size = h->size - dist;
	new_h->allocated = h->allocated;
	new_h->magic = HEAP_MAGIC;
	
	// update the old block
	h->size = dist;
	
	// insert the new block
	if(h->next != 0)
		h->next->prev = new_h;
	new_h->next = h->next;
	h->next = new_h;
	new_h->prev = h;
	
	return new_h;
}

// TODO: need to be able to expand the heap when it fills

// for debugs only
/*static void dump_heap()
{
	header_t* h = kheap;
	printf("kernel heap dump:\n");
	while(h)
	{
		printf("%x %x,%x,%x %x-%x\t", h, h->magic, h->size, h->allocated, h->prev, h->next);
		h = h->next;
	}
	printf("\n");
}*/

// only up to the first 4KiB of memory returned is
// guaranteed to be contiguous in physical memory
// and even then that's only if align is set
static void* kmalloc_int(size_t size, int align)
{
	header_t* h;
	header_t* new_h;
	header_t* back_h;
	
	if(size == 0)
		return 0;
	else
		size += size%4; // round up to nearest 4 bytes
	
	if(!kheap)
		kpanic("kmalloc: tried to allocate memory before heap initialization");
	
	h = kheap;
	while(h != 0)
	{
		if(h->magic != HEAP_MAGIC)
			kpanic("kmalloc: heap magic violated");
		
		// find a big enough free block
		if((h->allocated == 0) && (h->size >= size + sizeof(header_t)))
		{
			// TODO: can we consolidate the aligned and unaligned logic somehow?
			if(align)
			{
				// get the nearest page aligned address
				uint32_t aligned_addr = (uint32_t)((void*)h + 2*sizeof(header_t) + 0x0FFF) & 0xFFFFF000;
				
				// if the space before the align isnt big enough for a header, move on
				if(aligned_addr - (uint32_t)h < sizeof(header_t))
					continue;
				// if the space after the align isn't big enough, move on
				if(aligned_addr + size > (uint32_t)h + h->size)
					continue;
				
				// create the new block at the aligned address
				new_h = split_block(h, aligned_addr - (uint32_t)h - sizeof(header_t));
				new_h->allocated = 1;
				
				// chop off the excess?
				if(new_h->size > size + 2*sizeof(header_t))
				{
					back_h = split_block(new_h, sizeof(header_t) + size);
					back_h->allocated = 0;
				}
				
				return (void*)new_h + sizeof(header_t);
			}
			else
			{
				if(h->size < size + 2*sizeof(header_t))
				{
					// there won't be enough space left for the free
					// block header so we use the entire free block
					h->allocated = 1;
				}
				else
				{
					// otherwise we just use the free block and reduce
					// its size to match the request, then add a free
					// block after it for the remainder of the space
					new_h = split_block(h, sizeof(header_t) + size);
					h->allocated = 1;
				}
				
				return (void*)h + sizeof(header_t);
			}
		}
		// iterate to the next free block
		h = h->next;
	}
	
	// never found a big enough block, fail
	return 0;
}

void kheap_init(void* start, size_t size)
{
	kheap = (header_t*)start;
	kheap->size = size;
	kheap->allocated = 0;
	kheap->magic = HEAP_MAGIC;
	kheap->next = 0;
	kheap->prev = 0;
}

void heap_sanity_check()
{
	header_t* h = kheap;
	header_t* temp = 0;
	while(h)
	{
		if(h->magic != HEAP_MAGIC)
			kpanic("kheap: heap magic violated");
		/*if(h->next && (uint32_t)h + h->size != (uint32_t)h->next) // remove this since heap may not be contiguous
		{
			printf(">>>>> %x + %x != %x\n", h, h->size, h->next);
			kpanic("kheap: bad node size");
		}*/
		if(temp != h->prev)
			kpanic("kheap: bad prev pointer");
		temp = h;
		h = h->next;
	}
}

void heap_print_info()
{
	unsigned int free = 0;
	unsigned int used = 0;
	
	header_t* h = kheap;
	while(h)
	{
		if(h->allocated)
			used += h->size;
		else
			free += h->size;
		h = h->next;
	}
	printf("kernel heap\n");
	printf("\ttotal: %d KiB\n", (free + used)/1024);
	printf("\t free: %d KiB\n", free/1024);
	printf("\t used: %d KiB\n", used/1024);
}

void kfree(void* ptr)
{
	header_t* h;
	
	if(!ptr)
		return;
	
	h = (header_t*)(ptr - sizeof(header_t));
	if(h->magic != HEAP_MAGIC)
		kpanic("kfree: heap magic violated");
	h->allocated = 0;
	
	// merge with next block if it's free
	if(h->next && h->next->allocated == 0)
	{
		h->size += h->next->size;
		if(h->next->next)
			h->next->next->prev = h;
		h->next = h->next->next;
	}
	
	// merge with the prev block if it's free
	if(h->prev && h->prev->allocated == 0)
	{
		h->prev->size += h->size;
		if(h->next)
			h->next->prev = h->prev;
		h->prev->next = h->next;
	}
}

void* kmalloc(size_t size)
{
	return kmalloc_int(size, 0);
}

void* kmalloc_a(size_t size)
{
	return kmalloc_int(size, 1);
}

void* kmalloc_ap(size_t size, phys_addr* p)
{
	void* addr = kmalloc_int(size, 1);
	if(p)
		*p = virt_to_phys((virt_addr)addr);
	return addr;
}

void* kcalloc(size_t num, size_t size)
{
	void* ptr;
	
	ptr = kmalloc(num*size);
	if(!ptr)
		return 0;
	
	memset(ptr, 0, num*size);
	return ptr;
}
