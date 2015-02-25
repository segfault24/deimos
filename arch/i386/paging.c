#include <stdint.h>
#include <i386/paging.h>

/***** PAGE DIRECTORY UTILITY FUNCTIONS *****/
void pde_set_attrib(pd_entry* e, uint32_t attrib){*e |= attrib;}
void pde_clear_attrib(pd_entry* e, uint32_t attrib){*e &= ~attrib;}
uint32_t pde_get_attrib(pd_entry* e, uint32_t attrib){return *e & attrib;}
void pde_set_frame(pd_entry* e, phys_addr addr){*e = (*e & ~PDE_FRAME) | (addr & PDE_FRAME);}
phys_addr pde_get_frame(pd_entry* e){return *e & PDE_FRAME;}

pd_entry* pd_lookup(page_directory_t* pd, virt_addr addr)
{
	if(pd)
		return &(pd->entries[addr>>22 & 0x3FF]);
	return 0;
}

/***** PAGE TABLE UTILITY FUNCTIONS *****/
void pte_set_attrib(pt_entry* e, uint32_t attrib){*e |= attrib;}
void pte_clear_attrib(pt_entry* e, uint32_t attrib){*e &= ~attrib;}
uint32_t pte_get_attrib(pt_entry* e, uint32_t attrib){return *e & attrib;}
void pte_set_frame(pt_entry* e, phys_addr addr){*e = (*e & ~PTE_FRAME) | (addr & PTE_FRAME);}
phys_addr pte_get_frame(pt_entry* e){return *e & PTE_FRAME;}

pt_entry* pt_lookup(page_table_t* pt, virt_addr addr)
{
	if(pt)
		return &(pt->entries[addr>>12 & 0x3FF]);
	return 0;
}

