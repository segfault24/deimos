#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <kernel/tty.h>

void kmain()
{
    size_t i;

    term_init();
    for(i=0; i<10; i++)
    {
        term_puts("does it work? ");
    }
    term_scroll();
}

