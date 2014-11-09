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

# kernel heap
.section .kheap, "aw", @nobits
.global kheap_start
.global kheap_end
kheap_start:
.skip 1048576 # 1 MiB
kheap_end:

# kernel stack
.section .kstack, "aw", @nobits
.global kstack_top
.global kstack_bottom
kstack_top:
.skip 16384 # 16 KiB
kstack_bottom:

.section .text

# this is our entry point that is called by GRUB
# we need to grab the multiboot info structure address,
# drop back to real mode to get the memory map from BIOS,
# and then call the main kernel body, which will do what
# it needs to put us back in protected mode

#.extern detect_memory
.extern kmain
.global boot
boot:
	# setup our stack as the very first thing
	movl $kstack_bottom, %esp
	# ebx contains the multiboot info structure address
	pushl %ebx

	# drop to real mode

	# detect memory
	#call detect_memory

	# start the main body of the kernel
	call kmain
	# kmain shouldn't return, but just in case, hang
	cli
	jmp hang

hang:
	hlt
	jmp hang

