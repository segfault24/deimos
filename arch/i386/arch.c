#include <kernel/arch.h>
#include <i386/gdt.h>
#include <i386/idt.h>
#include <i386/isr.h>
#include <i386/pic.h>
#include <i386/irq.h>
#include <i386/pit.h>
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

void arch_init()
{
	// initialization MUST be done in these sequences
	// GDT -> IDT -> ISR -> PIC -> IRQ -> PIT

	disable_interrupts(); // just to be sure

	gdt_init();
	idt_init();
	pic_init();
	isr_init();
	irq_init();
	pit_init();

	enable_interrupts();
}
