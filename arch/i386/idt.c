#include <i386/idt.h>

idt_entry create_idt_entry(uint16_t selector, uint32_t offset, uint8_t flags)
{
	uint64_t desc;

	// upper half

	// shift to access lower half
	desc = desc<<32;

	// lower half

	return (idt_entry)desc;
}

void idt_init()
{
}
