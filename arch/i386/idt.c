#include <stdint.h>
#include <stddef.h>

#include <i386/idt.h>

static idt_entry idt[IDT_NUM_ENTRIES];

static idt_entry idt_create_entry(uint16_t selector, void (*offset), uint8_t flags)
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

	return (idt_entry)desc;
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
	iptr.size = IDT_NUM_ENTRIES*(sizeof(idt_entry)) - 1;
	iptr.offset = (uint32_t)&idt;

	// clear all the entries
	for(i=0; i<IDT_NUM_ENTRIES; i++)
	{
		idt[i] = idt_create_entry(0, 0, IDT_KERNEL_NULL);
	}

	// call our asm function to perform actual load
	idt_load();
}
