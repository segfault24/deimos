#ifndef I386_ISR_H
#define I386_ISR_H

#include <stdint.h>

// assembly wrapper stubs that call our
// generic interrupt handler
extern void isr0w();  // divide by zero
extern void isr1w();  // debug
extern void isr2w();  // non maskable interrupt
extern void isr3w();  // breakpoint
extern void isr4w();  // overflow
extern void isr5w();  // bound range exceeded
extern void isr6w();  // invalid opcode
extern void isr7w();  // device not available
extern void isr8w();  // double fault
extern void isr10w(); // invalid TSS
extern void isr11w(); // segment not present
extern void isr12w(); // stack segment fault
extern void isr13w(); // general protection fault
extern void isr14w(); // page fault
extern void isr16w(); // x87 floating point exception
extern void isr17w(); // alignment check
extern void isr18w(); // machine check
extern void isr19w(); // SIMD floating point exception
extern void isr20w(); // virtualization exception
extern void isr30w(); // security exception

// actual interrupt handling goes here
void isr_handler(uint8_t interrupt);

// initializes and registers ISRs with the IDT
void isr_init();

#endif
