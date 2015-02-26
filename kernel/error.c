#include <stddef.h>
#include <stdint.h>
#include <kernel/stdio.h>
#include <kernel/tty.h>
#include <kernel/error.h>

void kwarn(const char* message)
{
	tty_setcolor(VGA_YELLOW, VGA_BLACK);
	printf("\nWarning: %s\n", message);
}

void kerror(const char* message)
{
	tty_setcolor(VGA_LIGHT_RED, VGA_BLACK);
	printf("\nError: %s\n", message);
}

void kpanic(const char* message)
{
	tty_setcolor(VGA_RED, VGA_BLACK);
	printf("\nPanic: the kernel encountered an unrecoverable error\n");
	printf("Panic: >> %s\n", message);
	printf("Panic: system will now halt...");

	for(;;);
}
