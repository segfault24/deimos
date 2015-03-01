/*
 *  Copyright (C) 2015  Austin Selick-Bottos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <kernel/string.h>
#include <kernel/stdio.h>
#include <kernel/error.h>
#include <kernel/kalloc.h>
#include <i386/boot.h>
#include <i386/isr.h>
#include <i386/paging.h>

/********************************************/
/***** PAGE DIRECTORY UTILITY FUNCTIONS *****/
/********************************************/

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

/****************************************/
/***** PAGE TABLE UTILITY FUNCTIONS *****/
/****************************************/

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

/************************************/
/***** PRIMARY PAGING FUNCTIONS *****/
/************************************/

static page_directory_t __attribute__((aligned (4096))) _kernel_pd;

static page_directory_t* kernel_pd;
static page_directory_t* current_pd;

void paging_init()
{
	pd_entry* pde;
	
	// setup a new page directory to replace the bootstrapped one
	// this one does not have the identity mapping for 0-4MiB
	kernel_pd = &_kernel_pd;
	memset((void*)kernel_pd, 0, sizeof(page_directory_t));
	kernel_pd->phys = (phys_addr)((uint32_t)kernel_pd - KERNEL_VMA);
	
	// add the entry (mapping 0-4MiB to KERNEL_VMA)
	pde = pd_lookup(kernel_pd, KERNEL_VMA);
	pde_set_attrib(pde, PDE_PRESENT);
	pde_set_attrib(pde, PDE_WRITABLE);
	pde_set_attrib(pde, PDE_4MB);
	pde_set_frame(pde, 0);
	
	// switch to the new directory
	switch_directory(kernel_pd);
	
	// register the page fault handler
	register_isr(14, &page_fault_handler);
}

void switch_directory(page_directory_t* pd)
{
	current_pd = pd;
	__asm__ volatile ( "movl %0, %%cr3" : :"rm"(pd->phys) );
}

phys_addr virt_to_phys(virt_addr v)
{
	// grab the directory entry
	pd_entry* pde = pd_lookup(current_pd, v);
	// make sure it's valid
	if(!pde_get_attrib(pde, PDE_PRESENT))
		return 0;
	// support 4MiB pages
	if(pde_get_attrib(pde, PDE_4MB))
		return pde_get_frame(pde) | (v & 0x003FFFFF);
	
	// grab the page table entry
	pt_entry* pte = pt_lookup(current_pd->tables[v>>22 & 0x3FF], v);
	// make sure it's valid
	if(!pte_get_attrib(pte, PTE_PRESENT))
		return 0;
	// return the address
	return pte_get_frame(pte) | (v & ~PTE_FRAME);
}

void page_fault_handler(regs_t* regs)
{
	uint32_t cr2;
	
	// get the faulty address from CR2
	__asm__ volatile ("movl %%cr2, %0;" :"=rm"(cr2) : );
	printf("\npage fault at: %x with error code: %x\n", cr2, regs->err_code);
	dump_regs(regs);
	kpanic("page fault");
}

page_directory_t* get_kernel_pd()
{
	return kernel_pd;
}

page_directory_t* get_current_pd()
{
	return current_pd;
}

// TODO: this
page_directory_t* clone_pd(page_directory_t* pd)
{
	page_directory_t* new_pd;
	phys_addr paddr;
	
	new_pd = kmalloc_ap(sizeof(page_directory_t), &paddr);
	if(!new_pd)
		return 0;
	new_pd->phys = paddr;
	
	// loop through each entry and corresponding table
	//for(0, 1024, 1)
	//{
	//	new_pd->entries = ;
	//	new_pd->tables = ;
	//}
	
	return pd;
}
