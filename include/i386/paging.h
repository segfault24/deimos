#ifndef I386_PAGING_H
#define I386_PAGING_H

#include <stdint.h>

typedef uint32_t phys_addr;
typedef uint32_t virt_addr;

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
} page_directory_t;

void pde_set_attrib(pd_entry* e, uint32_t attrib);
void pde_clear_attrib(pd_entry* e, uint32_t attrib);
uint32_t pde_get_attrib(pd_entry* e, uint32_t attrib);

void pde_set_frame(pd_entry* e, phys_addr addr);
phys_addr pde_get_frame(pd_entry* e);

/* returns a pointer to the corresponding directory */
/* entry given a directory pointer and virtual addr */
pd_entry* pd_lookup(page_directory_t* pd, virt_addr vaddr);


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

#endif
