#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/error.h>
#include <i386/boot.h>
#include <i386/isr.h>
#include <i386/pmem_mgr.h>
#include <i386/vmem_mgr.h>

static page_directory* cur_page_dir = 0;
static phys_addr cur_page_dir_phys_addr = 0;

uint8_t vmem_mgr_alloc_page(pt_entry* e)
{
	phys_addr p = pmem_mgr_alloc();
	if(p)
	{
		pt_entry_set_frame(e, p);
		pt_entry_set_attrib(e, PT_ENTRY_PRESENT);
		return 1;
	}
	return 0;
}

void vmem_mgr_free_page(pt_entry* e)
{
	phys_addr p = pt_entry_get_frame(*e);
	if(p)
		pmem_mgr_free(p);
	pt_entry_remove_attrib(e, PT_ENTRY_PRESENT);
}

inline page_directory* vmem_mgr_get_directory()
{
	return cur_page_dir;
}

inline uint8_t vmem_mgr_switch_directory(page_directory* pd)
{
	if(pd)
	{
		cur_page_dir = pd;
		cur_page_dir_phys_addr = (phys_addr)&(pd->entries);
		__asm__ volatile (	"movl %0, %%cr3"
							: :"rm"(cur_page_dir_phys_addr) );
		return 1;
	}
	return 0;
}

void vmem_mgr_flush_tlb_entry(virt_addr addr)
{
	__asm__ volatile (	"cli;"
						"invlpg %0;"
						"sti;" : :"rm"(addr) );
}

/*************************************************/
/* THE REAL MAGIC HAPPENS IN THE FUNCTIONS BELOW */
/*************************************************/

// TODO: page faults
void vmem_mgr_pf_handler()
{
	uint32_t cr2;
	
	// get the faulty address from CR2
	__asm__ volatile ("movl %%cr2, %0;" :"=rm"(cr2) : );
	tty_putv("page fault at: ", cr2, "\n");
	while(1);
	// get a phys page
	// map it into the directory/table
}

void vmem_mgr_map_page(phys_addr paddr, virt_addr vaddr)
{
	page_directory* pd;
	pd_entry* pde;
	page_table* pt;
	pt_entry* pte;
	size_t i;

	// get the directory
	pd = vmem_mgr_get_directory();
	// get the directory entry
	pde = pd_lookup(pd, vaddr);

	// does the table exist?
	if(!pd_entry_is_present(*pde))
	{
		// the table is not present, create a new one
		pt = (page_table*)pmem_mgr_alloc();
		if(!pt)
		{
			// we couldnt allocate a new table
			// TODO: swapping?
			kpanic("out of physical memory");
			return;
		}

		// clear the table
		for(i=0; i<PT_PAGES_PER_TABLE; i++)
			pt->entries[i] = 0;

		// create a new directory entry for the table
		*pde = 0;
		pd_entry_set_frame(pde, (phys_addr)pt);
		pd_entry_set_attrib(pde, PD_ENTRY_WRITABLE);
		pd_entry_set_attrib(pde, PD_ENTRY_PRESENT);
	}
	// get the table and associated entry
	pt = (page_table*)(*pde & ~0xfff);
	pte = pt_lookup(pt, vaddr);

	// perform the mapping
	pt_entry_set_frame(pte, paddr);
	pt_entry_set_attrib(pte, PT_ENTRY_WRITABLE);
	pt_entry_set_attrib(pte, PT_ENTRY_PRESENT);

	// flush the corresponding TLB entry
	vmem_mgr_flush_tlb_entry(vaddr);
}

void vmem_mgr_init()
{
	phys_addr paddr;
	virt_addr vaddr;
	size_t i;
	pt_entry* pte;
	pd_entry* pde;

	page_table* pt;
	page_table* pt2;
	page_directory* pd;

	// allocate default page table
	pt = (page_table*)pmem_mgr_alloc();
	if(!pt)
		kpanic("could not allocate memory for kernel initialization");

	// allocate 3GiB page table
	pt2 = (page_table*)pmem_mgr_alloc();
	if(!pt2)
		kpanic("could not allocate memory for kernel initialization");

	// identity map 0-4MiB
	// we set every entry so clearing the table is unecessary
	paddr = 0x00000000;
	vaddr = 0x00000000;
	for(i=0; i<PT_PAGES_PER_TABLE; i++)
	{
		// set table entry
		pte = pt_lookup(pt2, vaddr);
		*pte = 0;
		pt_entry_set_frame(pte, paddr);
		// TODO: should this be writable?
		pt_entry_set_attrib(pte, PT_ENTRY_PRESENT);

		paddr += VMEM_PAGE_SIZE;
		vaddr += VMEM_PAGE_SIZE;
	}

	// map 0-4MiB to 3GiB
	paddr = 0x00000000;
	vaddr = KERNEL_VADDR;
	for(i=0; i<PT_PAGES_PER_TABLE; i++)
	{
		// set table entry
		pte = pt_lookup(pt, vaddr);
		*pte = 0;
		pt_entry_set_frame(pte, paddr);
		// TODO: should this be writable?
		pt_entry_set_attrib(pte, PT_ENTRY_PRESENT);

		paddr += VMEM_PAGE_SIZE;
		vaddr += VMEM_PAGE_SIZE;
	}

	// create default directory
	pd = (page_directory*)pmem_mgr_alloc();
	if(!pd)
		kpanic("could not allocate memory for kernel initialization");

	// clear the directory
	for(i=0; i<PD_TABLES_PER_DIR; i++)
		pd->entries[i] = 0;

	// add the tables to the directory
	pde = pd_lookup(pd, 0xC0000000);
	pd_entry_set_frame(pde, (phys_addr)pt);
	pd_entry_set_attrib(pde, PD_ENTRY_WRITABLE);
	pd_entry_set_attrib(pde, PD_ENTRY_PRESENT);

	pde = pd_lookup(pd, 0x00000000);
	pd_entry_set_frame(pde, (phys_addr)pt2);
	pd_entry_set_attrib(pde, PD_ENTRY_WRITABLE);
	pd_entry_set_attrib(pde, PD_ENTRY_PRESENT);

	// register our page fault handler, switch to
	// our directory, and finally enable paging
	isr_register_isr(14, &vmem_mgr_pf_handler);
	vmem_mgr_switch_directory(pd);
}
