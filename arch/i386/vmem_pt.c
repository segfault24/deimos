#include <stddef.h>
#include <stdint.h>
#include <i386/pmem_mgr.h>
#include <i386/vmem_pt.h>

inline void pt_entry_set_attrib(pt_entry* e, uint32_t attrib)
{
	*e |= attrib;
}

inline void pt_entry_remove_attrib(pt_entry* e, uint32_t attrib)
{
	*e &= ~attrib;
}

inline void pt_entry_set_frame(pt_entry* e, phys_addr addr)
{
	*e = (*e & ~PT_ENTRY_FRAME) | addr;
}

inline phys_addr pt_entry_get_frame(pt_entry e)
{
	return e & PT_ENTRY_FRAME;
}

inline uint8_t pt_entry_is_present(pt_entry e)
{
	return e & PT_ENTRY_PRESENT;
}

inline uint8_t pt_entry_is_writable(pt_entry e)
{
	return e & PT_ENTRY_WRITABLE;
}

inline pt_entry* pt_lookup(page_table* pt, virt_addr addr)
{
	if(pt)
		return &(pt->entries[addr>>12 & 0x3FF]);
	return 0;
}
