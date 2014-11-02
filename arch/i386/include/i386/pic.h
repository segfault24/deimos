#ifndef I386_PIC_H
#define I386_PIC_H

#define PIC_MASTER_CMD  0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD   0xA0
#define PIC_SLAVE_DATA  0xA1

#define PIC_INIT 0x11
#define PIC_MASTER_OFFSET 0x??
#define PIC_SLAVE_OFFSET 0x??
#define PIC_MASTER_CSCD 0x04
#define PIC_SLAVE_CSCD 0x02
#define PIC_8086 0x01

#define PIC_EOI  0x20

// APIC functions
// extern void pic_apic_present();

// initializes the PICs and then remaps
// the IRQs so we don't hide CPU exceptions
void pic_init();

// tell the PICs we're done
void pic_send_eoi();

#endif
