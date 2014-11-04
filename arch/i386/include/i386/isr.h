#ifndef I386_ISR_H
#define I386_ISR_H

#include <stdint.h>

// wrapper stubs that call our generic interrupt handler
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
// 32-47 are technically mapped IRQs
extern void isr32w(); // programmable interrupt timer
extern void isr33w(); // keyboard
extern void isr35w(); // COM2
extern void isr36w(); // COM1
extern void isr37w(); // LPT2
extern void isr38w(); // floppy disk
extern void isr39w(); // LPT1/spurious
extern void isr40w(); // CMOS real time clock
extern void isr41w(); // (free)
extern void isr42w(); // (free)
extern void isr43w(); // (free)
extern void isr44w(); // PS/2 mouse
extern void isr45w(); // FPU/coprocessor/inter-processor
extern void isr46w(); // primary ATA hard disk
extern void isr47w(); // secondary ATA hard disk

// generic interrupt handler for use only by the asm wrappers
void isr_handler(uint8_t interrupt, uint32_t error);

// These are wrappers for functions in i386/idt.h that also perform
// the necessary setup and clean up for the differences between
// CPU exceptions (int 0-31) and hardware IRQs (int 32-47).
//
// Registered ISRs should be in the form "void fname(uint32_t error)".
// These ISRs should be made as short as possible.
// Error numbers have no meaning for IRQs, and can be discarded.
void isr_register_isr(uint8_t interrupt, void (*func_ptr));
void isr_clear_isr(uint8_t interrupt);

void isr_init();

#endif
