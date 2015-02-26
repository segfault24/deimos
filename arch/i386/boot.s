#
#   Copyright (C) 2015  Austin Selick-Bottos
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

.global boot
.extern kmain

#.global kstack_top
#.global kstack_bottom

.set KERNEL_VMA, 0xC0000000 # 3GiB

# multiboot setup stuff
.set ALIGN,     1<<0                # aligning stuff on page boundaries
.set MEMINFO,   1<<1                # mem map?
.set FLAGS,     ALIGN | MEMINFO     # flag field for multiboot
.set MAGIC,     0x1BADB002          # multiboot magic number
.set CHECKSUM,  -(MAGIC + FLAGS)    # checksum

# multiboot header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# kernel stack
.section .kstack, "aw", @nobits
kstack_top:
.skip 0x4000 # 16 KiB
kstack_bottom:

# kernel page directory
.section .data
.align 0x1000
kernel_pd:
	.space 0x1000, 0x00

# This is our entry point that is called by GRUB.
# We need to grab the multiboot info structure address,
# setup paging, and jump to the higher kernel.
# In this area, we need to subtract the kernels virtual
# base because we link the kernel using its virtual home
# but we start in physical memory
.section .text
boot:
	# setup the stack (adjusting to physical addr)
	mov $kstack_bottom, %esp
	sub $KERNEL_VMA, %esp
	
	# ebx contains the multiboot info structure address
	# note that this is a physical address
	push %ebx
	
	### do the paging setup
	##################
	# if we want to get creative with paging setup later, see here:
	# http://wiki.osdev.org/User:Mduft/HigherHalf_Kernel_with_32-bit_Paging
	##################
	# identity mapping using 4MiB page table
	mov $kernel_pd, %eax		# get the page directory
	sub $KERNEL_VMA, %eax		# adjust to physical
	mov $0x00000083, %edx
	mov %edx, (%eax)			# set the directory entry
	
	# virtual mapping using 4MiB page table
	push %eax					# save the pd address
	mov $KERNEL_VMA, %eax		# get the virtual offset
	shr $22, %eax
	mov $4, %ecx
	mul %ecx
	mov %eax, %edx				# edx now contains the index offset for the second entry
	pop %eax					# restore the pd address
	add %edx, %eax				# move to the correct pd entry
	mov $0x00000083, %edx
	mov %edx, (%eax)			# set the directory entry
	
	# enable 4MiB pages
	mov %cr4, %ecx
	or $0x00000010, %ecx
	mov %ecx, %cr4
	
	# load the kernel page directory
	mov $kernel_pd, %ecx
	sub $KERNEL_VMA, %ecx
	mov %ecx, %cr3
	
	# enable paging
	mov %cr0, %ecx
	or $0x80000000, %ecx
	mov %ecx, %cr0
	
	# long jump to the higher half of the kernel
	lea higherhalfboot, %ecx
	jmp *%ecx

higherhalfboot:
	# adjust the stack to its virtual home
	add $KERNEL_VMA, %esp
	mov %esp, %ebp
	
	# **removed this section so we can access the multiboot
	#   struct properly from the higher kernel
	# we can now unmap the identity mapping
	#mov $kernel_pd, %eax
	#mov $0x00, %edx
	#mov %edx, (%eax)
	#invlpg (0)
	
	# jump into our main kernel C code
	call kmain
	hlt

##################
# kernel heap
#.section .kheap, "aw", @nobits
#.global kheap_start
#.global kheap_end
#kheap_start:
#.skip 0x100000 # 1 MiB
#kheap_end:
