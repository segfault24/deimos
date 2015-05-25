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

#include <stddef.h>
#include <stdint.h>

#include <kernel/stdio.h>
#include <kernel/string.h>
#include <kernel/error.h>
#include <kernel/multiboot.h>
#include <kernel/tty.h>
#include <kernel/arch.h>
#include <kernel/mm.h>
#include <kernel/task.h>
#include <kernel/sched.h>
#include <kernel/pci.h>
#include <kernel/net.h>
#include <kernel/vfs.h>

#include <kernel/dsh.h>

// assumptions from multiboot spec:
//   - EBX contains the address of the multiboot info structure
//   - A20 is gate enabled
//   - paging is disabled
//   - we are in protected mode
//   - we are not in virtual 8086 mode
//   - interrupts are disabled
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

void kmain(multiboot_info_t* mbt)
{
	tty_init(); // terminal
	arch_init(); // architecture unique stuff
	mm_init(mbt); // memory management setup
	sched_init(); // scheduling setup
	
	pci_init(); // scan the pci bus
	net_init(); // sets up the networking stack
	vfs_init(); // virtual file system setup
	
	create_kernel_task(dsh_loop);
	
	// idle loop
	while(1)
		sched_yield();
}
