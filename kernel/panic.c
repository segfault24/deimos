#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/panic.h>

void kernel_panic(char* message)
{
	tty_setcolor(VGA_RED, VGA_WHITE);
	tty_puts("Panic: the kernel encountered an unrecoverable error\n");
	tty_puts("Panic: >> ");
	tty_puts(message);
	tty_puts("\nPanic: system will now halt...");

	for(;;);
}
