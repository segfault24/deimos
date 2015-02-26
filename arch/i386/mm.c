#include <stdint.h>

#include <kernel/multiboot.h>
#include <kernel/stdio.h>
#include <kernel/error.h>
#include <kernel/kalloc.h>
#include <kernel/mm.h>

#include <i386/boot.h>
#include <i386/frame.h>
#include <i386/paging.h>
#include <i386/vmem_mgr.h>

//extern void* kheap_start;
//extern void* kheap_end;

void mm_init(multiboot_info_t* mbt)
{
	// check if the memory map is present
	if(!(mbt->flags & 0x20))
		kpanic("no memory map available in multiboot info structure");
	
	// initialize the frame allocator
	frame_init(mbt);
	
	// initialize the virtual memory manager
	// this maps the kernel to its virtual home
	vmem_mgr_init();
	
	
	// setup the kernel heap
	// TODO: these need to be virtual addresses, not physical...
	// TODO: we should probably update the vmem_mgr's static variables too...
	//kheap_init(&kheap_start, &kheap_end);
	
	// TESTING BELOW, DELETE WHEN DONE
	//while(1)
	//{
	//	tty_putv("alloc:", (uint32_t)kmalloc(1024*64-4), " ");
	//	kheap_available();
	//	tty_getchar();
	//}
	//kheap_available();
	//void* test1 = kmalloc(1024*512);tty_putv("alloc:", (uint32_t)test1, "\n");
	//kheap_available();
	//void* test2 = kmalloc(1024*512);tty_putv("alloc:", (uint32_t)test2, "\n");
	//kheap_available();
	//kfree(test1);tty_putv("free:", (uint32_t)test1, "\n");
	//kheap_available();
	//kfree(test2);tty_putv("free:", (uint32_t)test2, "\n");
	//kheap_available();
}

/////////////////////////////////////////////////////////////////////
//print_page_directory(vmem_mgr_get_directory());

/*void print_page_table(pd_entry pde, size_t pdi, page_table* pt)
{
	pt_entry pte;
	size_t i;

	for(i=0; i<PT_PAGES_PER_TABLE; i+=16)
	{
		pte = pt->entries[i];
		if(pt_entry_is_present(pte))
		{
			putv("PDE: ", (uint32_t)pde, "  ");
			putv("PTE: ", (uint32_t)pte, "  ");
			putv("VADDR: ", (uint32_t)(4096*(pdi*1024+i)), "  ");
			putv("PADDR: ", (uint32_t)(pte & 0xFFFFF000), "\n");
		}
		tty_getchar();
	}
}

void print_page_directory(page_directory* pd)
{
	pd_entry pde;
	page_table* pt;
	size_t i;

	for(i=0; i<PD_TABLES_PER_DIR; i++)
	{
		pde = pd->entries[i];
		if(pd_entry_is_present(pde))
		{
			pt = (page_table*)(pde & ~0xfff);
			print_page_table(pde, i, pt);
		}
	}
}*/