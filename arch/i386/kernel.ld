OUTPUT_FORMAT(elf32-i386)
ENTRY(boot)

KERNEL_PMA = 0x00100000;
KERNEL_VMA = 0xC0000000;

SECTIONS
{
	/*	everything will be linked as if it lives
		at 0xC0100000 but will be actually loaded
		into physical memory at 0x00100000 */
	. = KERNEL_PMA + KERNEL_VMA;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VMA) {
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VMA) {
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VMA) {
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VMA) {
		*(COMMON)
		*(.bss)
		*(.kstack)
	}
}

