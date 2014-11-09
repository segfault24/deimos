#include <i386/ioasm.h>
#include <i386/pic.h>

void pic_init()
{
	// start the initialization sequence
	outb(PIC_MASTER_CMD,  PIC_INIT);
	outb(PIC_SLAVE_CMD,   PIC_INIT);
	iowait();
	// set the new vector offset
	outb(PIC_MASTER_DATA, PIC_MASTER_OFFSET);
	outb(PIC_SLAVE_DATA,  PIC_SLAVE_OFFSET);
	iowait();
	// tell the PICs how they are cascaded
	outb(PIC_MASTER_DATA, PIC_MASTER_CSCD);
	outb(PIC_SLAVE_DATA,  PIC_SLAVE_CSCD);
	iowait();
	// set to 8086 mode
	outb(PIC_MASTER_DATA, PIC_8086);
	outb(PIC_SLAVE_DATA, PIC_8086);
	iowait();
	// set the interrupt mask
	outb(PIC_MASTER_DATA, PIC_MASTER_MASK);
	outb(PIC_SLAVE_DATA, PIC_SLAVE_MASK);
	iowait();

	// make sure IRQ 2 is enabled, so that the
	// slave PIC can talk to the master PIC
	// TODO: is this actually necessary?
	pic_unmask_irq(2);
}

void pic_mask_irq(uint8_t irq)
{
	uint16_t port;
	uint8_t mask;

	if(irq<8)
	{
		port = PIC_MASTER_DATA;
	} else {
		port = PIC_SLAVE_DATA;
		irq -= 8;
	}
	// get the current mask
	mask = inb(port);
	// update the mask
	mask = mask | (1<<irq);
	// write out the new mask
	outb(port, mask);
}

void pic_unmask_irq(uint8_t irq)
{
	uint16_t port;
	uint8_t mask;

	if(irq<8)
	{
		port = PIC_MASTER_DATA;
	} else {
		port = PIC_SLAVE_DATA;
		irq -= 8;
	}
	// get the current mask
	mask = inb(port);
	// update the mask
	mask = mask & ~(1<<irq);
	// write out the new mask
	outb(port, mask);
}

uint8_t pic_read_mask()
{
	return (inb(PIC_SLAVE_DATA) << 8) | inb(PIC_MASTER_DATA);
}

uint16_t pic_read_reg(uint8_t pic_reg)
{
	outb(PIC_MASTER_CMD, pic_reg);
	outb(PIC_SLAVE_CMD, pic_reg);
	return (inb(PIC_SLAVE_CMD) << 8) | inb(PIC_MASTER_CMD);
}

void pic_send_eoi(uint8_t irq)
{
	if(irq >= 8)
	{
		outb(PIC_SLAVE_CMD, PIC_EOI);
	}
	outb(PIC_MASTER_CMD, PIC_EOI);
}

/*void pic_disable()
{
	outb(PIC_MASTER_DATA, PIC_DISABLE);
	outb(PIC_SLAVE_DATA, PIC_DISABLE);
}*/
