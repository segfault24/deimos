.global irq0w
.global irq1w
.global irq2w
.global irq3w
.global irq4w
.global irq5w
.global irq6w
.global irq7w
.global irq8w
.global irq9w
.global irq10w
.global irq11w
.global irq12w
.global irq13w
.global irq14w
.global irq15w

.align 4
.extern irq_handler

# TODO: do we need to load the kernel data
#       segment descriptor before calling
#       the common handler?

irq1w: # keyboard interrupt
	pushal
	pushl $0 # dummy
	pushl $1
	call irq_handler
	addl $8, %esp
	popal
	iret
