.global isr0w
.global isr1w
.global isr2w
.global isr3w
.global isr4w
.global isr5w
.global isr6w
.global isr7w
.global isr8w
.global isr10w
.global isr11w
.global isr12w
.global isr13w
.global isr14w
.global isr16w
.global isr17w
.global isr18w
.global isr19w
.global isr20w
.global isr30w

.align 4
.extern isr_handler

# TODO: do we need to load the kernel data
#       segment descriptor before calling
#       the common handler?

isr8w: # double fault
	pushal
	push $8
	call isr_handler
	addl $8, %esp
	popal
	iret

isr13w: # general protection fault
	pushal
	pushl $13
	call isr_handler
	addl $8, %esp
	popal
	iret
