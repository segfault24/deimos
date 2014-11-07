#include <stddef.h>
#include <stdint.h>
#include <kernel/multiboot.h>
#include <kernel/arch.h>
#include <kernel/tty.h>
#include <kernel/panic.h>

// assumptions from multiboot spec:
//   - EBX contains the address of the multiboot info structure
//   - A20 is gate enabled
//   - paging is disabled
//   - we are in protected mode
//   - we are not in virtual 8086 mode
//   - interrupts are disabled
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

extern void* kstack_top;
extern void* kstack_bottom;
extern void* kheap_start;
extern void* kheap_end;

void kmain(multiboot_info_t* mbt)
{
	tty_init();
	arch_init(mbt);

	//tty_puts("kheap_start:   ");
	//tty_puti((uint32_t)&kheap_start);
	//tty_puts("\nkheap_end:     ");
	//tty_puti((uint32_t)&kheap_end);
	//tty_puts("\nkstack_top:    ");
	//tty_puti((uint32_t)&kstack_top);
	//tty_puts("\nkstack_bottom: ");
	//tty_puti((uint32_t)&kstack_bottom);
	//tty_puts("\n");

	for(;;)
		tty_putchar(tty_getchar());
}
