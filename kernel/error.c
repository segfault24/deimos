#include <stddef.h>
#include <stdint.h>
#include <kernel/stdio.h>
#include <kernel/tty.h>
#include <kernel/error.h>

void kwarn(const char* message)
{
	tty_setcolor(VGA_YELLOW, VGA_BLACK);
	puts("\nWarning: %s\n", message);
}

void kerror(const char* message)
{
	tty_setcolor(VGA_LIGHT_RED, VGA_BLACK);
	puts("\nError: %s\n", message);
}

void kpanic(const char* message)
{
	tty_setcolor(VGA_RED, VGA_BLACK);
	printf("\nPanic: the kernel encountered an unrecoverable error\n");
	printf("Panic: >> %s\n", message);
	printf("\nPanic: system will now halt...");

	for(;;);
}
