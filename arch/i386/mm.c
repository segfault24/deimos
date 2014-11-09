#include <stdint.h>

#include <kernel/multiboot.h>
#include <kernel/tty.h>
#include <kernel/error.h>

#include <i386/boot.h>
#include <i386/pmem_mgr.h>
#include <i386/vmem_mgr.h>

void mm_init(multiboot_info_t* mbt)
{
	// initialize the physical memory manager
	pmem_mgr_init();

	// check if the memory map is present
	if(!(mbt->flags & 0x20))
		kpanic("no memory map available in multiboot info structure");

	// get the multiboot memory map and its length
	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mbt->mmap_addr;
	multiboot_uint32_t end = mbt->mmap_addr + mbt->mmap_length;

	// parse the memory map and configure the physical memory manager
	while((multiboot_uint32_t)mmap < end)
	{
		// mark regions labeled "available" as free for use
		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
			pmem_mgr_free_region(mmap->addr, mmap->addr+mmap->len-1);
		// move to next entry
		mmap = (multiboot_memory_map_t*)
			((unsigned int)mmap + mmap->size + sizeof(unsigned int));
	}

	// make sure the kernel's physical location is reserved
	pmem_mgr_reserve_region(KERNEL_PADDR, KERNEL_PADDR+KERNEL_MSIZE-1);

	// initialize the virtual memory manager
	vmem_mgr_init();

	// map the kernel to its virtual home
	//vmem_mgr_map_kernel();

	// enable paging
	vmem_mgr_enable_paging();
}