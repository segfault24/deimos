#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/pit.h>
#include <i386/irq.h>

#include <kernel/tty.h>

void irq_handler(uint8_t irq, uint32_t error)
{
	if(irq == 1)
	{
		term_puts("-");
		term_puti((uint32_t)irq);
		term_putchar(' ');
		term_puti((uint32_t)error);
		term_putchar('\n');
	} else {
	}
	pic_send_eoi(irq);
}

void irq_init()
{
	idt_register_isr(32, (uint32_t)&irq0w);
	pic_unmask_irq(0);

	idt_register_isr(33, (uint32_t)&irq1w);
	pic_unmask_irq(1);
}
