#include <stddef.h>
#include <stdint.h>
#include <kernel/string.h>
#include <kernel/kalloc.h>

// TODO: this

void kheap_init(void* start, void* end)
{
	start++;
	end++;
}

void* kmalloc(size_t size)
{
	size++;
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
	ptr++;
}
