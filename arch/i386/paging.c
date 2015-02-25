#include <stdint.h>
#include <i386/paging.h>

/***** PAGE DIRECTORY UTILITY FUNCTIONS *****/
void pde_set_attrib(pd_entry* e, uint32_t attrib){*e |= attrib;}
void pde_clear_attrib(pd_entry* e, uint32_t attrib){*e &= ~attrib;}
uint32_t pde_get_attrib(pd_entry* e, uint32_t attrib){return *e & attrib;}
void pde_set_frame(pd_entry* e, void* paddr){*e = (*e & ~PDE_FRAME) | (paddr & PDE_FRAME);}
void* pde_get_frame(pd_entry* e){return *e & PDE_FRAME;}

pd_entry* pd_lookup(page_directory* pd, void* vaddr)
{
	if(pd)
		return &(pd->entries[vaddr>>22 & 0x3FF]);
	return 0;
}

/***** PAGE TABLE UTILITY FUNCTIONS *****/
void pte_set_attrib(pt_entry* e, uint32_t attrib){*e |= attrib;}
void pte_clear_attrib(pt_entry* e, uint32_t attrib){*e &= ~attrib;}
uint32_t pte_get_attrib(pt_entry* e, uint32_t attrib){return *e & attrib;}
void pte_set_frame(pt_entry* e, void* paddr){*e = (*e & ~PTE_FRAME) | (paddr & PTE_FRAME);}
void* pte_get_frame(pt_entry* e){return *e & PTE_FRAME;}

pt_entry* pt_lookup(page_table* pt, void* vaddr)
{
	if(pt)
		return &(pt->entries[vaddr>>12 & 0x3FF]);
	return 0;
}

