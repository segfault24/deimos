#include <kernel/arch.h>
#include <kernel/tty.h>

#include <i386/gdt.h>
#include <i386/idt.h>
#include <i386/isr.h>
#include <i386/pit.h>
#include <i386/ps2.h>
#include <i386/ps2_kbd.h>

// Assumptions from multiboot spec:
//   - EBX contains the address of the multiboot info structure
//   - A20 is gate enabled
//   - paging is disabled
//   - we are in protected mode
//   - we are not in virtual 8086 mode
//   - interrupts are disabled
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

/*void print_page_table(pd_entry pde, size_t pdi, page_table* pt)
{
	pt_entry pte;
	size_t i;

	for(i=0; i<PT_PAGES_PER_TABLE; i+=16)
	{
		pte = pt->entries[i];
		if(pt_entry_is_present(pte))
		{
			putv("PDE: ", (uint32_t)pde, "  ");
			putv("PTE: ", (uint32_t)pte, "  ");
			putv("VADDR: ", (uint32_t)(4096*(pdi*1024+i)), "  ");
			putv("PADDR: ", (uint32_t)(pte & 0xFFFFF000), "\n");
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
			print_page_table(pde, i, pt);
		}
	}
}*/

void arch_init()
{
	gdt_init(); // setup the global tables with the cpu
	idt_init();
	isr_init(); // setup the various components of the interrupt system

	pit_init(); // setup the interrupt timer for a sysclock
	ps2_init(); // setup the ps/2 controller(s)
	kbd_init(); // setup the ps/2 keyboard

	isr_enable_interrupts(); // now we're ready

	//print_page_directory(vmem_mgr_get_directory());
}
