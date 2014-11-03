#include <i386/idt.h>
#include <i386/isr.h>

#include <kernel/tty.h>

void isr_handler(uint8_t interrupt)
{
	term_puts("int: ");
	term_puti((uint32_t)interrupt);
	term_putchar('\n');
}

void isr_init()
{
	idt_register_isr(8, (uint32_t)&isr8w);
	idt_register_isr(13, (uint32_t)&isr13w);
}
