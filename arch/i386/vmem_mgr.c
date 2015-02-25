#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/error.h>
#include <i386/boot.h>
#include <i386/isr.h>
#include <i386/pmem_mgr.h>
#include <i386/vmem_mgr.h>

static page_directory_t* cur_page_dir = 0;
static phys_addr cur_page_dir_phys = 0;

uint8_t vmem_mgr_alloc_page(pt_entry* e)
{
	phys_addr p = pmem_mgr_alloc();
	if(p)
	{
		pte_set_frame(e, p);
		pte_set_attrib(e, PTE_PRESENT);
		return 1;
	}
	return 0;
}

void vmem_mgr_free_page(pt_entry* e)
{
	phys_addr p = pte_get_frame(e);
	if(p)
		pmem_mgr_free(p);
	pte_clear_attrib(e, PTE_PRESENT);
}

page_directory_t* vmem_mgr_get_directory()
{
	return cur_page_dir;
}

uint8_t vmem_mgr_switch_directory(page_directory_t* pd)
{
	if(pd)
	{
		cur_page_dir = pd;
		cur_page_dir_phys = (phys_addr)&(pd->entries);
		__asm__ volatile (	"movl %0, %%cr3"
							: :"rm"(cur_page_dir_phys) );
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
	page_directory_t* pd;
	pd_entry* pde;
	page_table_t* pt;
	pt_entry* pte;
	size_t i;

	// get the directory
	pd = vmem_mgr_get_directory();
	// get the directory entry
	pde = pd_lookup(pd, vaddr);

	// does the table exist?
	if(!pde_get_attrib(pde, PDE_PRESENT))
	{
		// the table is not present, create a new one
		pt = (page_table_t*)pmem_mgr_alloc();
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
		pde_set_frame(pde, (phys_addr)pt);
		pde_set_attrib(pde, PDE_WRITABLE);
		pde_set_attrib(pde, PDE_PRESENT);
	}
	// get the table and associated entry
	pt = (page_table_t*)(*pde & ~0xfff);
	pte = pt_lookup(pt, vaddr);

	// perform the mapping
	pte_set_frame(pte, paddr);
	pte_set_attrib(pte, PTE_WRITABLE);
	pte_set_attrib(pte, PTE_PRESENT);

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

	page_table_t* pt;
	page_table_t* pt2;
	page_directory_t* pd;

	// allocate default page table
	pt = (page_table_t*)pmem_mgr_alloc();
	if(!pt)
		kpanic("could not allocate memory for kernel initialization");

	// allocate 3GiB page table
	pt2 = (page_table_t*)pmem_mgr_alloc();
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
		pte_set_frame(pte, paddr);
		// TODO: should this be writable?
		pte_set_attrib(pte, PTE_PRESENT);

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
		pte_set_frame(pte, paddr);
		// TODO: should this be writable?
		pte_set_attrib(pte, PTE_PRESENT);

		paddr += VMEM_PAGE_SIZE;
		vaddr += VMEM_PAGE_SIZE;
	}

	// create default directory
	pd = (page_directory_t*)pmem_mgr_alloc();
	if(!pd)
		kpanic("could not allocate memory for kernel initialization");

	// clear the directory
	for(i=0; i<PD_TABLES_PER_DIR; i++)
		pd->entries[i] = 0;

	// add the tables to the directory
	pde = pd_lookup(pd, 0xC0000000);
	pde_set_frame(pde, (phys_addr)pt);
	pde_set_attrib(pde, PDE_WRITABLE);
	pde_set_attrib(pde, PDE_PRESENT);

	pde = pd_lookup(pd, 0x00000000);
	pde_set_frame(pde, (phys_addr)pt2);
	pde_set_attrib(pde, PDE_WRITABLE);
	pde_set_attrib(pde, PDE_PRESENT);

	// register our page fault handler, switch to
	// our directory, and finally enable paging
	isr_register_isr(14, &vmem_mgr_pf_handler);
	vmem_mgr_switch_directory(pd);
}
