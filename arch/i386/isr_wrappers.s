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

.global isr0w,  isr1w,  isr2w,  isr3w,  isr4w,  isr5w
.global isr6w,  isr7w,  isr8w,          isr10w, isr11w
.global isr12w, isr13w, isr14w,         isr16w, isr17w
.global isr18w, isr19w, isr20w,                 isr30w
.global isr32w, isr33w, isr34w, isr35w, isr36w, isr37w
.global isr38w, isr39w, isr40w, isr41w, isr42w, isr43w
.global isr44w, isr45w, isr46w, isr47w

.align 4
.extern isr_handler

# TODO: do we need to load the kernel data (or possibly code)
#       segment descriptor before calling the common handler?

# shared interrupt return stub
common:
	cli
	pushal # pushes eax,ecx,edx,ebx,esp,ebp,esi,edi
	
	call isr_handler
	
	popal # pops eax,ecx,edx,ebx,esp,ebp,esi,edi
	addl $8, %esp # clean up the interrupt number and error code
	sti
	iret

# cpu exceptions
isr0w: # divide by zero
	push $0 # dummy
	push $0
	jmp common

isr1w: # debug
	push $0 # dummy
	push $1
	jmp common

isr2w: # non-maskable interrupt
	push $0 # dummy
	push $2
	jmp common

isr3w: # breakpoint
	push $0 # dummy
	push $3
	jmp common

isr4w: # overflow
	push $0 # dummy
	push $4
	jmp common

isr5w: # bound range exceeded
	push $0 # dummy
	push $5
	jmp common

isr6w: # invalid opcode
	push $0 # dummy
	push $6
	jmp common

isr7w: # device not available
	push $0 # dummy
	push $7
	jmp common

isr8w: # double fault
	push $8
	jmp common

isr10w: # invalid TSS
	push $10
	jmp common

isr11w: # segment not present
	push $11
	jmp common

isr12w: # stack segment fault
	push $12
	jmp common

isr13w: # general protection fault
	pushl $13
	jmp common

isr14w: # page fault
	pushl $14
	jmp common

isr16w: # x87 floating point exception
	pushl $0 # dummy
	pushl $16
	jmp common

isr17w: # alignment check
	pushl $17
	jmp common

isr18w: # machine check
	pushl $0 # dummy
	pushl $18
	jmp common

isr19w: # SIMD floating point exception
	pushl $0 # dummy
	pushl $19
	jmp common

isr20w: # virtualization exception
	pushl $0 # dummy
	pushl $20
	jmp common

isr30w: # security exception
	pushl $30
	jmp common





# hardware interrupt requests

isr32w: # programmable interrupt timer
	pushl $0 # dummy
	pushl $32
	jmp common

isr33w: # keyboard interrupt
	pushl $0 # dummy
	pushl $33
	jmp common
