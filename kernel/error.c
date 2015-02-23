#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/error.h>

void kwarn(const char* message)
{
	tty_setcolor(VGA_YELLOW, VGA_BLACK);
	tty_puts("\nWarning: ");
	tty_puts(message);
	tty_puts("\n");
}

void kerror(const char* message)
{
	tty_setcolor(VGA_LIGHT_RED, VGA_BLACK);
	tty_puts("\nError: ");
	tty_puts(message);
	tty_puts("\n");
}

void kpanic(const char* message)
{
	tty_setcolor(VGA_RED, VGA_BLACK);
	tty_puts("\nPanic: the kernel encountered an unrecoverable error\n");
	tty_puts("Panic: >> ");
	tty_puts(message);
	tty_puts("\nPanic: system will now halt...");

	for(;;);
}
