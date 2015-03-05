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

#include <stdint.h>
#include <stddef.h>

#include <i386/idt.h>

// calls the lidt asm instruction
extern void idt_load();

idt_ptr_t iptr;
static idt_entry_t idt[IDT_NUM_ENTRIES];

static idt_entry_t idt_create_entry(uint16_t selector, void (*offset), uint8_t flags)
{
	uint64_t desc;
	uint32_t offset_t;
	desc = 0;
	offset_t = (uint32_t)offset;

	// upper half
	desc |= offset_t     & 0xFFFF0000; // offset 16:31
	desc |= flags<<8     & 0x0000FF00; // flags

	// shift to access lower half
	desc = desc<<32;

	// lower half
	desc |= selector<<16 & 0xFFFF0000; // selector 0:15
	desc |= offset_t     & 0x0000FFFF; // offset 0:15

	return (idt_entry_t)desc;
}

void idt_register_isr(uint8_t interrupt, void (*func_ptr))
{
	idt[interrupt] = idt_create_entry(0x08, func_ptr, IDT_KERNEL_INT);
}

void idt_clear_isr(uint8_t interrupt)
{
	idt[interrupt] = idt_create_entry(0, 0, IDT_KERNEL_NULL);
}

void idt_init()
{
	size_t i;

	// setup the pointer to the table
	iptr.size = IDT_NUM_ENTRIES*(sizeof(idt_entry_t)) - 1;
	iptr.offset = (uint32_t)&idt;

	// clear all the entries
	for(i=0; i<IDT_NUM_ENTRIES; i++)
	{
		idt[i] = idt_create_entry(0, 0, IDT_KERNEL_NULL);
	}

	// call our asm function to perform actual load
	idt_load();
}
