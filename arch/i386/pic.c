#include <i386/pic.h>
#include <i386/ioasm.h>

void pic_init()
{
	// start the initialization sequence
	outb(PIC_MASTER_CMD,  PIC_INIT);
	outb(PIC_SLAVE_CMD,   PIC_INIT);

	// set the new vector offset
	outb(PIC_MASTER_DATA, PIC_MASTER_OFFSET);
	outb(PIC_SLAVE_DATA,  PIC_SLAVE_OFFSET);

	// tell the PICs how they are cascaded
	outb(PIC_MASTER_DATA, PIC_MASTER_CSCD);
	outb(PIC_SLAVE_DATA,  PIC_SLAVE_CSCD);

	// set to 8086 mode
	outb(PIC_MASTER_DATA, PIC_8086);
	outb(PIC_SLAVE_DATA, PIC_8086);

	// set the interrupt mask
	outb(PIC_MASTER_DATA, PIC_MASTER_MASK);
	outb(PIC_SLAVE_DATA, PIC_SLAVE_MASK);
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
