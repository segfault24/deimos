.global idt_load
.extern iptr

idt_load:
	lidt iptr
	ret
