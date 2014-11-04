.global isr0w, isr1w, isr2w, isr3w, isr4w
.global isr5w, isr6w, isr7w, isr8w, isr10w
.global isr11w, isr12w, isr13w, isr14w, isr16w
.global isr17w, isr18w, isr19w, isr20w, isr30w
.global isr32w, isr33w, isr34w, isr35w, isr36w
.global isr37w, isr38w, isr39w, isr40w, isr41w
.global isr42w, isr43w, isr44w, isr45w, isr46w
.global isr47w

.align 4
.extern isr_handler

# TODO: do we need to load the kernel data (or possibly code)
#       segment descriptor before calling the common handler?

# shared interrupt return stub
cmnret:
	addl $8, %esp
	popal
	iret

# cpu exceptions
isr8w: # double fault
	pushal
	push $8
	call isr_handler
	jmp cmnret

isr13w: # general protection fault
	pushal
	pushl $13
	call isr_handler
	jmp cmnret

# hardware interrupt requests
isr32w: # programmable interrupt timer
	pushal
	pushl $0 # dummy
	pushl $32
	call isr_handler
	jmp cmnret

isr33w: # keyboard interrupt
	pushal
	pushl $0 # dummy
	pushl $33
	call isr_handler
	jmp cmnret;
