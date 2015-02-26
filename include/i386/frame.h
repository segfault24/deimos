#ifndef I386_FRAME_H
#define I386_FRAME_H

#include <stddef.h>
#include <kernel/multiboot.h>
#include <i386/paging.h>

// architecture specified frame size
#define FRAME_SIZE  4096
// max possible frames in a 4GiB system
#define MAX_FRAMES  1024*1024

// we parse the multiboot memory map here
// this establishes our available physical memory
void frame_init(multiboot_info_t* mbt);

// allocates a physical frame
// returns 0 if there are none available
phys_addr alloc_frame();

// freeing addresses not allocated produces undefined behavior
void free_frame(phys_addr addr);

// for getting the status of the physical memory
size_t num_total_frames();
size_t num_free_frames();
size_t num_used_frames();

#endif
