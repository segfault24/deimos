#ifndef I386_PIC_H
#define I386_PIC_H

#include <stddef.h>
#include <stdint.h>

#define PIC_MASTER_CMD  0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD   0xA0
#define PIC_SLAVE_DATA  0xA1

#define PIC_INIT 0x11
#define PIC_8086 0x01
#define PIC_EOI  0x20

// maps IRQs 0-7 to interrupts 32-39
#define PIC_MASTER_OFFSET 0x20
// maps IRQs 8-15 to interrupts 40-47
#define PIC_SLAVE_OFFSET 0x28

#define PIC_MASTER_CSCD 0x04
#define PIC_SLAVE_CSCD 0x02

// mask all IRQs by default
#define PIC_MASTER_MASK 0xFF
#define PIC_SLAVE_MASK 0xFF

// APIC functions
// extern void pic_apic_present();
// void pic_disable();

// initializes the PICs and then remaps
// the IRQs so we don't hide CPU exceptions
void pic_init();

// sets the masks on the individual IRQ lines
void pic_mask_irq(uint8_t irq);
void pic_unmask_irq(uint8_t irq);

// tell the PICs we're done
void pic_send_eoi(uint8_t irq);

#endif
