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

# this file contains direct assembly functions that cannot be inlined with gcc

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

.global idt_load
.extern iptr
idt_load:
	lidt iptr
	ret

.global read_eip
read_eip:
	pop %eax
	jmp *%eax

