#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/irq.h>

#include <kernel/tty.h>

void irq_handler(uint8_t irq)
{
	term_puts("irq: ");
	term_puti((uint32_t)irq);
	term_putchar('\n');

	pic_send_eoi(irq);
}

void irq_init()
{
	idt_register_isr(33, (uint32_t)&irq1w);
	pic_unmask_irq(1);
}
