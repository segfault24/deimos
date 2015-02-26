#include <kernel/tty.h>
#include <kernel/stdio.h>

int puts(const char* str)
{
	tty_puts(str);
	return 0;
}
