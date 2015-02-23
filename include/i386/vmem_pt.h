#ifndef I386_VMEM_PT_H
#define I386_VMEM_PT_H

#include <stdint.h>
#include <i386/pmem_mgr.h>

#define PT_ENTRY_PRESENT  0x1
#define PT_ENTRY_WRITABLE 0x2
#define PT_ENTRY_USERMODE 0x4
#define PT_ENTRY_PWT      0x8
#define PT_ENTRY_PCD      0x10
#define PT_ENTRY_ACCESS   0x20
#define PT_ENTRY_DIRTY    0x40

#define PT_ENTRY_GLOBAL   0x100;
#define PT_ENTRY_FRAME    0xFFFFF000

#define PT_PAGES_PER_TABLE 1024

typedef uint32_t pt_entry;

typedef struct {
	pt_entry entries[PT_PAGES_PER_TABLE];
} page_table;

/* set entry fields */
inline void pt_entry_set_attrib(pt_entry* e, uint32_t attrib);
inline void pt_entry_remove_attrib(pt_entry* e, uint32_t attrib);
inline void pt_entry_set_frame(pt_entry* e, phys_addr addr);

/* retrieve entry fields */
inline phys_addr pt_entry_get_frame(pt_entry e);
inline uint8_t pt_entry_is_present(pt_entry e);
inline uint8_t pt_entry_is_writable(pt_entry e);

/* returns a pointer to the corresponding table */
/* entry given a table pointer and a virtual addr */
inline pt_entry* pt_lookup(page_table* pt, virt_addr addr);

#endif
