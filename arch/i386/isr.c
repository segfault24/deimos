#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/isr.h>

#include <kernel/tty.h>

void isr_handler(uint8_t interrupt)
{
	//term_puts("caught interrupt: ");
	term_puti((uint32_t)interrupt);
	term_putchar(' ');

	// if it was an IRQ, tell the PICs we're done
	if(interrupt > 31) {
		pic_send_eoi(interrupt);
	}
}

void isr_init()
{
	idt_register_isr(0, (uint32_t)&isr0w);
	idt_register_isr(1, (uint32_t)&isr1w);
	idt_register_isr(13, (uint32_t)&isr13w);

	idt_register_isr(33, (uint32_t)&irq1w);
	//pic_unmask_irq(1);
}
