#ifndef I386_ISR_H
#define I386_ISR_H

// assembly wrapper stubs that call our
// generic interrupt handler
extern void isr0w();
extern void isr1w();
extern void isr13w();

extern void irq1w();

// meat of interrupt handling goes here
void isr_handler(uint8_t interrupt);

// initializes and registers ISRs with the IDT
void isr_init();

#endif
