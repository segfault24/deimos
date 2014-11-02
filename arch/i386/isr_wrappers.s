.global isr0w
.global isr1w
.global isr13w

.global irq1w

.align 4
.extern isr_handler

# TODO: do we need to load the kernel data
#       segment descriptor before calling
#       the common handler?

isr0w: # divide by zero error
	pushal
	push 0 # dummy
	push 0
	call isr_handler
	popal
	iret

isr1w: # debug
	pushal
	push 0 # dummy
	push 1
	call isr_handler
	popal
	iret

isr13w: # general protection fault
	pushal
	push 13
	call isr_handler
	popal
	iret

irq1w: # keyboard interrupt
	pushal
	push 0 # dummy
	push 33
	call isr_handler
	popal
	iret
