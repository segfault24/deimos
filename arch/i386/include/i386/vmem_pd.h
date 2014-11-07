#ifndef I386_VMEM_PD_H
#define I386_VMEM_PD_H

#include <stddef.h>
#include <stdint.h>
#include <i386/pmem_mgr.h>

#define PD_ENTRY_PRESENT  0x1
#define PD_ENTRY_WRITABLE 0x2
#define PD_ENTRY_USERMODE 0x4
#define PD_ENTRY_PWT      0x8
#define PD_ENTRY_PCD      0x10
#define PD_ENTRY_ACCESS   0x20
#define PD_ENTRY_DIRTY    0x40
#define PD_ENTRY_4MB      0x80
#define PD_ENTRY_GLOBAL   0x100;
#define PD_ENTRY_FRAME    0xFFFFF000

#define PD_TABLES_PER_DIR 1024

typedef uint32_t pd_entry;

typedef struct {
	pd_entry entries[PD_TABLES_PER_DIR];
} page_directory;

/* set entry fields */
inline void pd_entry_set_attrib(pd_entry* e, uint32_t attrib);
inline void pd_entry_remove_attrib(pd_entry* e, uint32_t attrib);
inline void pd_entry_set_frame(pd_entry* e, phys_addr addr);

/* retrieve entry fields */
inline phys_addr pd_entry_get_frame(pd_entry e);
inline uint8_t pd_entry_is_present(pd_entry e);
inline uint8_t pd_entry_is_writable(pd_entry e);
inline uint8_t pd_entry_is_usermode(pd_entry e);
inline uint8_t pd_entry_is_4mb(pd_entry e);

/* returns a pointer to the corresponding directory */
/* entry given a directory pointer and virtual addr */
inline pd_entry* pd_lookup(page_directory* pd, virt_addr addr);

#endif
