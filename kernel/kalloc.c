#include <stddef.h>
#include <stdint.h>

#include <kernel/stdio.h>
#include <kernel/string.h>
#include <kernel/kalloc.h>

// TODO: improve this

typedef struct _freeblk_t {
	size_t size;
	struct _freeblk_t* next;
	struct _freeblk_t* prev;
} freeblk_t;

typedef struct _allochdr_t {
	size_t size;
} allochdr_t;

static freeblk_t* freelist;

void kheap_init(void* start, void* end)
{
	freelist = (freeblk_t*)start;
	freelist->size = end-start;
	freelist->next = 0;
	freelist->prev = 0;
}

size_t kheap_available()
{
	size_t available;
	freeblk_t* blk;
	
	blk = freelist;
	available = 0;
	//tty_puts("heap:");
	while(blk != 0)
	{
		//tty_putv("", (uint32_t)blk, "-");
		//tty_putv("", (uint32_t)blk+blk->size, ">");
		
		available += blk->size;
		blk = blk->next;
	}
	//tty_putv(" size:", available, "\n");
	return available;
}

void* kmalloc(size_t size)
{
	freeblk_t* blk;
	allochdr_t* hdr;

	if(size == 0)
		return 0;

	blk = freelist;
	while(blk != 0)
	{
		if(blk->size >= size+sizeof(allochdr_t))
		{
			// found a big enough free block
			if(blk->size-size-sizeof(allochdr_t) >= sizeof(freeblk_t))
			{
				// just reduce the free block's size
				blk->size = blk->size-size-sizeof(allochdr_t);
				hdr = (void*)blk+blk->size;
			}
			else
			{
				return 0;
				// there won't be enough space left for the free
				// block header so we use the entire free block
				//size = blk->size-sizeof(allochdr);
				//if(blk->next != 0)
				//	blk->next->prev = blk->prev;
				//if(blk->prev != 0)
				//	blk->prev->next = blk->next;
				//hdr = (void*)blk;
				//
				// wipe the block
				//blk->size = 0;
				//blk->next = 0;
				//blk->prev = 0;
			}
			hdr->size = size;
			return (void*)hdr+sizeof(allochdr_t);
		}
		// iterate to the next free block
		blk = blk->next;
	}

	// never found a big enough block, fail
	return 0;
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

void kfree(void* ptr)
{
	freeblk_t* blk;
	freeblk_t* cur;
	allochdr_t* hdr;
	
	hdr = (void*)ptr-sizeof(allochdr_t);
	blk = (void*)hdr;
	blk->size = hdr->size + sizeof(allochdr_t);
	blk->next = 0;
	blk->prev = 0;
	
	// TODO: insert it back into the free list in order
	// currently it just appends the free blocks
	if(freelist == 0)
	{
		freelist = blk;
		return;
	}
	
	cur = freelist;
	while(cur->next != 0)
		cur = cur->next;
	
	cur->next = blk;
	blk->prev = cur;
}
