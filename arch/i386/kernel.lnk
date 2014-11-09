ENTRY(boot)
SECTIONS
{
	. = 1M; /* kernel load address */

	.text ALIGN(4K) :
	{
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) :
	{
		*(.rodata)
	}

	.data ALIGN(4K) :
	{
		*(.data)
	}

	.bss ALIGN(4K) :
	{
		*(COMMON)
		*(.bss)
		*(.kheap)
		*(.kstack)
	}
}

