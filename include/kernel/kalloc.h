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

#ifndef KERNEL_KALLOC_H
#define KERNEL_KALLOC_H

#include <stddef.h>
#include <i386/paging.h>

void kheap_init(void* start, size_t size);
void kheap_print();

void* kmalloc(size_t size);
void* kmalloc_a(size_t size);
void* kmalloc_ap(size_t size, phys_addr* p);
void* kcalloc(size_t num, size_t size);

void kfree(void* ptr);

#endif
