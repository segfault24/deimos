#include <kernel/arch.h>
#include <i386/gdt.h>
#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/isr.h>
#include <i386/pit.h>
#include <i386/ps2.h>
#include <i386/directasm.h>

#include <kernel/tty.h>

// Assumptions from multiboot spec:
//   - EBX contains the address of the multiboot info structure
//   - A20 is gate enabled
//   - paging is disabled
//   - we are in protected mode
//   - we are not in virtual 8086 mode
//   - interrupts are disabled
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

// Breakdown of interrupts on x86
//
// -INTERRUPTS (0-256)
//  +-EXCEPTIONS (0-31)
//  | +-FAULTS (0-1,5-7,9-14,16-17,19-20)
//  | +-TRAPS (3-4)
//  | +-ABORTS (8,18)
//  | +-(RESERVED) (15,21-31)
//  +-IRQS (mapped to 32-47)
//  +-SOFTWARE

void nullfunc(){}

void arch_init()
{
	// initializations MUST be done as in the sequence below
	// otherwise we might end up trying to add ISRs to an
	// improperly initialized IDT or cause triple faults

	disable_interrupts(); // just to be sure

	// setup the global tables with the cpu
	gdt_init();
	idt_init();

	// setup the various components of the interrupt system
	// register ISRs with the isr_register_isr call in i386/isr.h
	pic_init();
	isr_init();

	isr_register_isr(32, &nullfunc);
	//isr_register_isr(33, &kbd_isr);

	// setup the interrupt timer for a sysclock
	pit_init();

	// setup the ps/2 controller(s)
	ps2_init();

	// now we're finally ready to start receiving interrupts
	// (cross your fingers)
	enable_interrupts();
}
