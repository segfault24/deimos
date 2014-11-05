#include <stddef.h>
#include <stdint.h>
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
//	size_t i;

	tty_init();
	arch_init();
	tty_init2();

	for(;;);
//	for(i=0;;i++); // loop forever
//	{
//		if(i==1){term_putchar('.');}
//		if(i==0x00FFFFFF){i = 0;}
//	}
}
