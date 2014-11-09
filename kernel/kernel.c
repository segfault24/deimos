#include <stddef.h>
#include <stdint.h>

#include <kernel/multiboot.h>
#include <kernel/tty.h>
#include <kernel/mm.h>
#include <kernel/kalloc.h>
#include <kernel/arch.h>
#include <kernel/error.h>
#include <kernel/string.h>

// assumptions from multiboot spec:
//   - EBX contains the address of the multiboot info structure
//   - A20 is gate enabled
//   - paging is disabled
//   - we are in protected mode
//   - we are not in virtual 8086 mode
//   - interrupts are disabled
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

void kmain(multiboot_info_t* mbt)
{
	tty_init(); // terminal
	mm_init(mbt); // memory managers
	kheap_init(); // kernel heap
	arch_init(); // architecture unique stuff

	for(;;)
		tty_putchar(tty_getchar());
}
