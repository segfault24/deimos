#include <kernel/arch.h>
#include <i386/gdt.h>
#include <i386/idt.h>
#include <i386/isr.h>
#include <i386/pit.h>
#include <i386/ps2.h>
#include <i386/pmem_mgr.h>
#include <i386/vmem_mgr.h>

#include <kernel/tty.h>

// Assumptions from multiboot spec:
//   - EBX contains the address of the multiboot info structure
//   - A20 is gate enabled
//   - paging is disabled
//   - we are in protected mode
//   - we are not in virtual 8086 mode
//   - interrupts are disabled
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

void print_page_table(pd_entry pde, page_table* pt)
{
	pt_entry pte;
	size_t i;

	for(i=0; i<PT_PAGES_PER_TABLE; i++)
	{
		pte = pt->entries[i];
		if(pt_entry_is_present(pte))
		{
			tty_puts("PDE: ");
			tty_puti((uint32_t)pde);
			tty_puts("  PTE: ");
			tty_puti((uint32_t)pte);
			tty_puts("  \n");
		}
		tty_getchar();
	}
}

void print_page_directory(page_directory* pd)
{
	pd_entry pde;
	page_table* pt;
	size_t i;

	for(i=0; i<PD_TABLES_PER_DIR; i++)
	{
		pde = pd->entries[i];
		if(pd_entry_is_present(pde))
		{
			pt = (page_table*)(pde & ~0xfff);
			print_page_table(pde, pt);
		}
	}
}

void arch_init()
{
	// initializations MUST be done as in the sequence below
	// otherwise we might end up trying to add ISRs to an
	// improperly initialized IDT or cause bad things
	disable_interrupts(); // just to be sure
	gdt_init(); // setup the global tables with the cpu
	idt_init();
	isr_init(); // setup the various components of the interrupt system
	pit_init(); // setup the interrupt timer for a sysclock
	ps2_init(); // setup the ps/2 controller(s)

	// TODO: properly detect & reserve memory
	pmem_mgr_init(PMEM_MAX_BLOCKS);
	pmem_mgr_reserve_region(0x00000000, 0x00400000);

	// TODO: test the virtual memory manager
	vmem_mgr_init();

	enable_interrupts(); // now we're ready

	print_page_directory(vmem_mgr_get_directory());
}
