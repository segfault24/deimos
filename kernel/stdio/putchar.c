#include <kernel/tty.h>
#include <kernel/stdio.h>

int putchar(int character)
{
	tty_putchar(character);
	return character;
}
