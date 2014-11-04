#include <stddef.h>
#include <stdint.h>
#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/isr.h>

static uint8_t handler_exists[IDT_NUM_ENTRIES];
static void (*specific_handler[IDT_NUM_ENTRIES]) (uint32_t error);

void isr_handler(uint8_t interrupt, uint32_t error)
{
	// if there is a handler registered with us, call it
	if(handler_exists[interrupt])
	{
		(specific_handler[interrupt])(error);
	}

	// if the interrupt was an IRQ, tell the PIC we're done
	if(interrupt > 31 && interrupt < 48)
	{
		pic_send_eoi(interrupt - 32);
	}
}

void isr_register_isr(uint8_t interrupt,  void (*func_ptr))
{
	specific_handler[interrupt] = func_ptr;
	handler_exists[interrupt] = 1;
	// if the interrupt is an IRQ, tell the PIC to unmask the line
	if(interrupt > 31 && interrupt < 48)
	{
		pic_unmask_irq(interrupt - 32);
	}
}

void isr_clear_isr(uint8_t interrupt)
{
	specific_handler[interrupt] = 0;
	handler_exists[interrupt] = 0;
	// if the interrupt is an IRQ, tell the PIC to mask the line
	if(interrupt > 31 && interrupt < 48)
	{
		pic_mask_irq(interrupt - 32);
	}
}

void isr_init()
{
	size_t i;
	for(i=0; i<IDT_NUM_ENTRIES; i++)
	{
		isr_clear_isr(i);
	}

	// we tell the IDT to tell us about *all* interrupts
	// we check if we have a handler registered in our table
	// this is because we dont want to hard code ISR addresses
	// and GCC doesn't support writing ISRs in C (we need iret)
	//idt_register_isr(0,  &isr0w);
	//idt_register_isr(1,  &isr1w);
	//idt_register_isr(2,  &isr2w);
	//idt_register_isr(3,  &isr3w);
	//idt_register_isr(4,  &isr4w);
	//idt_register_isr(5,  &isr5w);
	//idt_register_isr(6,  &isr6w);
	//idt_register_isr(7,  &isr7w);
	idt_register_isr(8,  &isr8w);
	//idt_register_isr(10, &isr10w);
	//idt_register_isr(11, &isr11w);
	//idt_register_isr(12, &isr12w);
	idt_register_isr(13, &isr13w);
	//idt_register_isr(14, &isr14w);
	//idt_register_isr(16, &isr16w);
	//idt_register_isr(17, &isr17w);
	//idt_register_isr(18, &isr18w);
	//idt_register_isr(19, &isr19w);
	//idt_register_isr(20, &isr20w);
	//idt_register_isr(30, &isr30w);

	// now the IRQs
	idt_register_isr(32, &isr32w);
	idt_register_isr(33, &isr33w);
	//idt_register_isr(34, &isr34w);
	//idt_register_isr(35, &isr35w);
	//idt_register_isr(36, &isr36w);
	//idt_register_isr(37, &isr37w);
	//idt_register_isr(38, &isr38w);
	//idt_register_isr(39, &isr39w);
	//idt_register_isr(40, &isr40w);
	//idt_register_isr(41, &isr41w);
	//idt_register_isr(42, &isr42w);
	//idt_register_isr(43, &isr43w);
	//idt_register_isr(44, &isr44w);
	//idt_register_isr(45, &isr45w);
	//idt_register_isr(46, &isr46w);
	//idt_register_isr(47, &isr47w);
}
