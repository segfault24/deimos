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

#ifndef I386_PAGING_H
#define I386_PAGING_H

#include <stdint.h>
#include <i386/isr.h>

typedef uint32_t phys_addr;
typedef uint32_t virt_addr;


/****************************************/
/***** PAGE TABLE UTILITY FUNCTIONS *****/
/****************************************/

#define PTE_PRESENT			0x1
#define PTE_WRITABLE		0x2
#define PTE_USERMODE		0x4
#define PTE_WRITE_THROUGH	0x8
#define PTE_CACHE_DISABLE	0x10
#define PTE_ACCESS			0x20
#define PTE_DIRTY			0x40
#define PTE_GLOBAL			0x100;
#define PTE_FRAME			0xFFFFF000

#define PT_PAGES_PER_TABLE 1024

typedef uint32_t pt_entry;

typedef struct {
	pt_entry entries[PT_PAGES_PER_TABLE];
} page_table_t;

void pte_set_attrib(pt_entry* e, uint32_t attrib);
void pte_clear_attrib(pt_entry* e, uint32_t attrib);
uint32_t pte_get_attrib(pt_entry* e, uint32_t attrib);

void pte_set_frame(pt_entry* e, phys_addr addr);
phys_addr pte_get_frame(pt_entry* e);

/* returns a pointer to the corresponding table */
/* entry given a table pointer and a virtual addr */
pt_entry* pt_lookup(page_table_t* pt, virt_addr addr);


/********************************************/
/***** PAGE DIRECTORY UTILITY FUNCTIONS *****/
/********************************************/

#define PDE_PRESENT			0x1
#define PDE_WRITABLE		0x2
#define PDE_USERMODE		0x4
#define PDE_WRITE_THROUGH	0x8
#define PDE_CACHE_DISABLE	0x10
#define PDE_ACCESS			0x20
#define PDE_DIRTY			0x40
#define PDE_4MB				0x80
#define PDE_GLOBAL			0x100;
#define PDE_FRAME			0xFFFFF000

#define PD_TABLES_PER_DIR 1024

typedef uint32_t pd_entry;

typedef struct {
	pd_entry entries[PD_TABLES_PER_DIR];
	page_table_t* tables[PD_TABLES_PER_DIR];
	phys_addr phys;
} page_directory_t;

void pde_set_attrib(pd_entry* e, uint32_t attrib);
void pde_clear_attrib(pd_entry* e, uint32_t attrib);
uint32_t pde_get_attrib(pd_entry* e, uint32_t attrib);

void pde_set_frame(pd_entry* e, phys_addr addr);
phys_addr pde_get_frame(pd_entry* e);

/* returns a pointer to the corresponding directory */
/* entry given a directory pointer and virtual addr */
pd_entry* pd_lookup(page_directory_t* pd, virt_addr vaddr);


/****************************/
/***** PAGING FUNCTIONS *****/
/****************************/

void paging_init();

void switch_directory(page_directory_t* pd);

phys_addr virt_to_phys(virt_addr v);

void page_fault_handler(regs_t* regs);

#endif
