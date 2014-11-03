#ifndef I386_IRQ_H
#define I386_IRQ_H

#include <stdint.h>

// assembly wrapper stubs that call our
// generic irq handler
extern void irq0w();  // programmable interrupt timer
extern void irq1w();  // keyboard
extern void irq3w();  // COM2
extern void irq4w();  // COM1
extern void irq5w();  // LPT2
extern void irq6w();  // floppy disk
extern void irq7w();  // LPT1/spurious
extern void irq8w();  // CMOS real time clock
extern void irq9w();  // (free)
extern void irq10w(); // (free)
extern void irq11w(); // (free)
extern void irq12w(); // PS/2 mouse
extern void irq13w(); // FPU/coprocessor/inter-processor
extern void irq14w(); // primary ATA hard disk
extern void irq15w(); // secondary ATA hard disk

// actual interrupt handling goes here
void irq_handler(uint8_t irq);

// initializes and registers IRQs with the IDT
void irq_init();

#endif
