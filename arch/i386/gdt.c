#include <i386/gdt.h>

static gdt_entry_t gdt[GDT_NUM_ENTRIES];

static gdt_entry_t gdt_create_entry(uint32_t base, uint32_t limit, uint16_t flags)
{
	uint64_t desc;

	// upper half
	desc = 0;
	desc |= base         & 0xFF000000; // base 24:31
	desc |= flags<<8     & 0x00F00000; // flags
	desc |= limit        & 0x000F0000; // limit 16:19
	desc |= flags<<8     & 0x0000FF00; // access
	desc |= base>>16     & 0x000000FF; // base 16:23

	// shift to access lower half
	desc = desc<<32;

	// lower half
	desc |= base<<16     & 0xFFFF0000; // base 0:15
	desc |= limit        & 0x0000FFFF; // limit 0:15

	return (gdt_entry_t)desc;
}

void gdt_init()
{
	gptr.size = GDT_NUM_ENTRIES*(sizeof(gdt_entry_t)) - 1;
	gptr.offset = (uint32_t)&gdt;

	// null descriptor
	gdt[0] = gdt_create_entry(0, 0, 0);
	// kernel code segment (selector 0x08)
	gdt[1] = gdt_create_entry(0, 0xFFFFFFFF, GDT_KERNEL_CODE);
	// kernel data segment (selector 0x10)
	gdt[2] = gdt_create_entry(0, 0xFFFFFFFF, GDT_KERNEL_DATA);
	// userspace code segment (selector 0x18)
	gdt[3] = gdt_create_entry(0, 0xFFFFFFFF, GDT_USER_CODE);
	// userspace data segment (selector 0x20)
	gdt[4] = gdt_create_entry(0, 0xFFFFFFFF, GDT_USER_DATA);
	// call our asm function to perform actual load
	gdt_load();
}
