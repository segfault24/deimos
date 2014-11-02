#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/arch.h>
#include <kernel/tty.h>

// assumptions from multiboot spec:
//   - EBX contains the address of the multiboot info structure
//   - A20 is gate enabled
//   - paging is disabled
//   - we are in protected mode
//   - we are not in virtual 8086 mode
//   - interrupts are disabled
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

void kmain()
{
    term_init();
	term_puts("term init...\n");
	term_puts("term done\n");
	term_puts("arch init...\n");
	arch_init();
	term_puts("arch done\n");
	// __asm__ volatile ( "int $33" );
	while(1);
}
