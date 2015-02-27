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

#include <kernel/stdio.h>
#include <kernel/string.h>
#include <kernel/kalloc.h>

typedef struct _header_t {
	size_t size;
	int allocated;
	struct _header_t* prev;
	struct _header_t* next;
} header_t;

static header_t* kheap;

// only up to the first 4KiB of memory returned is
// guaranteed to be contiguous in physical memory
// and even then that's only if align is set
static void* kmalloc_int(size_t size, int align, phys_addr* p)
{
	header_t* h;
	header_t* new_h;
	
	if(size == 0 || align != 0 || p != 0)
		return 0;
	
	h = kheap;
	while(h != 0)
	{
		// find a big enough free block
		if((h->allocated == 0) && (h->size >= size + sizeof(header_t)))
		{
			if(h->size < size + 2*sizeof(header_t))
			{
				// there won't be enough space left for the free
				// block header so we use the entire free block
				h->allocated = 1;
				return (void*)h + sizeof(header_t);
			}
			
			// otherwise we just use the free block and reduce
			// its size to match the request, then add a free
			// block after it for the remainder of the space
			new_h = (void*)h + size + sizeof(header_t);
			new_h->size = h->size - (size + sizeof(header_t));
			new_h->allocated = 0;
			
			h->size = sizeof(header_t) + size;
			h->allocated = 1;
			
			// insert the new free block into the list
			if(h->next != 0)
				h->next->prev = new_h;
			new_h->next = h->next;
			h->next = new_h;
			new_h->prev = h;
			
			return (void*)h + sizeof(header_t);
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
	kheap->next = 0;
	kheap->prev = 0;
}

void kheap_print()
{
	header_t* h = kheap;
	while(h)
	{
		printf(" %x %x %x %x %x\n", h, h->prev, h->next, h->allocated, h->size);
		h = h->next;
	}
	printf("\n");
}

void kfree(void* ptr)
{
	header_t* h;
	
	h = (header_t*)(ptr - sizeof(header_t));
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
	return kmalloc_int(size, 0, 0);
}

void* kmalloc_a(size_t size)
{
	return kmalloc_int(size, 1, 0);
}

void* kmalloc_ap(size_t size, phys_addr* p)
{
	return kmalloc_int(size, 1, p);
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
