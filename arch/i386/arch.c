#include <kernel/arch.h>
#include <i386/gdt.h>
#include <i386/idt.h>
#include <i386/isr.h>
#include <i386/pit.h>
#include <i386/ps2.h>
#include <i386/pmem_mgr.h>

#include <kernel/tty.h>

// Assumptions from multiboot spec:
//   - EBX contains the address of the multiboot info structure
//   - A20 is gate enabled
//   - paging is disabled
//   - we are in protected mode
//   - we are not in virtual 8086 mode
//   - interrupts are disabled
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

void test_stat()
{
	tty_puts("    Free:");
	tty_puti(pmem_free_blocks());
	tty_puts(" Used:");
	tty_puti(pmem_used_blocks());
	tty_puts(" Total:");
	tty_puti(pmem_total_blocks());
	tty_puts("\n");
}

void* test_alloc()
{
	void* ptr = pmem_mgr_alloc();
	tty_puts("Alloc block: ");
	tty_puti((uint32_t)ptr);
	test_stat();
	return ptr;
}

void test_free(void* ptr)
{
	pmem_mgr_free(ptr);
	tty_puts("Freed block: ");
	tty_puti((uint32_t)ptr);
	test_stat();
}

void arch_init()
{
	//size_t i;
	//void* ptrs[64];

	// initializations MUST be done as in the sequence below
	// otherwise we might end up trying to add ISRs to an
	// improperly initialized IDT or cause bad things
	disable_interrupts(); // just to be sure
	gdt_init(); // setup the global tables with the cpu
	idt_init();
	isr_init(); // setup the various components of the interrupt system
	pit_init(); // setup the interrupt timer for a sysclock
	ps2_init(); // setup the ps/2 controller(s)

	// TODO: test the physical memory manager
	//pmem_mgr_init(1024); // setup the low level memory manager
	//pmem_mgr_reserve_region((void*)0, (void*)(16*4096-1));
	//test_stat();
	//for(i=0; i<1024; i++)
	//	ptrs[i] = test_alloc();
	//test_free(ptrs[0]);

	enable_interrupts(); // now we're ready
}
