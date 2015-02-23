#ifndef KERNEL_KALLOC_H
#define KERNEL_KALLOC_H

#include <stddef.h>

void kheap_init(void* start, void* end);
size_t kheap_available();

void* kmalloc(size_t size);
void* kcalloc(size_t num, size_t size);

void kfree(void* ptr);

#endif
