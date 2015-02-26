#include <kernel/arch.h>

#include <i386/gdt.h>
#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/isr.h>
#include <i386/pit.h>
#include <i386/ps2.h>
#include <i386/ps2_kbd.h>

void arch_init()
{
	gdt_init(); // setup the global descriptor table
	idt_init(); // setup the interrupt descriptor table
	pic_init(); // must do this before isr_init()
	isr_init(); // setup the various components of the interrupt system

	pit_init(); // setup the interrupt timer for a sysclock
	ps2_init(); // setup the ps/2 controller(s)
	kbd_init(); // setup the ps/2 keyboard

	enable_interrupts(); // now we're ready
}
