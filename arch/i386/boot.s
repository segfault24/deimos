# multiboot setup stuff
.set ALIGN,     1<<0                # aligning stuff on page boundaries
.set MEMINFO,   1<<1                # mem map?
.set FLAGS,     ALIGN | MEMINFO     # flag field for multiboot
.set MAGIC,     0x1BADB002          # multiboot magic number
.set CHECKSUM,  -(MAGIC + FLAGS)    # checksum

# here be the actual multiboot section
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# setup the stack?
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# finally we call the kernel
.section .text
.extern kmain
.global kmain_asm
.type kmain_asm, @function
kmain_asm:
    movl $stack_top, %esp
    call kmain
    cli
    hlt

hang:
	jmp hang
