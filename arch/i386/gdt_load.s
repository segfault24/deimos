.global gdt_load
.extern gptr

gdt_load:
	lgdt gptr
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ljmp $0x08, $gdt_helper

gdt_helper:
	ret

