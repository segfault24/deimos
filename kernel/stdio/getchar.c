#include <kernel/tty.h>
#include <kernel/stdio.h>

int getchar()
{
	return tty_getchar();
}
