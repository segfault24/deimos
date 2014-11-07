#include <stddef.h>
#include <stdint.h>
#include <i386/pmem_mgr.h>
#include <i386/vmem_pd.h>

inline void pd_entry_set_attrib(pd_entry* e, uint32_t attrib)
{
	*e |= attrib;
}

inline void pd_entry_remove_attrib(pd_entry* e, uint32_t attrib)
{
	*e &= ~attrib;
}

inline void pd_entry_set_frame(pd_entry* e, phys_addr addr)
{
	*e = (*e & ~PD_ENTRY_FRAME) | addr;
}

inline phys_addr pd_entry_get_frame(pd_entry e)
{
	return e & PD_ENTRY_FRAME;
}

inline uint8_t pd_entry_is_present(pd_entry e)
{
	return (e & PD_ENTRY_PRESENT) ? 1 : 0;
}

inline uint8_t pd_entry_is_writable(pd_entry e)
{
	return (e & PD_ENTRY_WRITABLE) ? 1 : 0;
}

inline uint8_t pd_entry_is_usermode(pd_entry e)
{
	return (e & PD_ENTRY_USERMODE) ? 1 : 0;
}

inline uint8_t pd_entry_is_4mb(pd_entry e)
{
	return (e & PD_ENTRY_4MB) ? 1 : 0;
}

inline pd_entry* pd_lookup(page_directory* pd, virt_addr addr)
{
	if(pd)
		return &(pd->entries[addr>>22 & 0x3FF]);
	return 0;
}
