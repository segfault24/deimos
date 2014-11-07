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

# finally we call the kernel
.section .text
.extern kmain
.global kmain_asm
.type kmain_asm, @function
kmain_asm:
    movl $kstack_bottom, %esp
	pushl %ebx
    call kmain
    cli
    jmp hang

hang:
	hlt
	jmp hang

