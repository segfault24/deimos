/*
 *  Copyright (C) 2015  Austin Selick-Bottos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stddef.h>
#include <stdint.h>

#include <kernel/stdio.h>
#include <kernel/error.h>
#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/isr.h>

static uint8_t handler_exists[IDT_NUM_ENTRIES];
static void (*specific_handler[IDT_NUM_ENTRIES]) (regs_t* regs);

void isr_handler(regs_t regs)
{
	// if there is a handler registered with us, call it
	if(handler_exists[regs.int_no])
	{
		(specific_handler[regs.int_no])(&regs);
	} else {
		printf("\nINT:%x ERR:%x\n", regs.int_no, regs.err_code);
		dump_regs(&regs);
		kpanic("uncaught interrupt");
	}

	// if the interrupt was an IRQ, tell the PIC we're done
	if(regs.int_no > 31 && regs.int_no < 48)
		pic_send_eoi(regs.int_no - 32);
}

void enable_interrupts(){__asm__ volatile ( "sti" );}
void disable_interrupts(){__asm__ volatile ( "cli" );}

void dump_regs(regs_t* regs)
{
	uint32_t cr0, cr2, cr3, cr4;
	
	__asm__ volatile ("movl %%cr0, %0;" :"=rm"(cr0) : );
	__asm__ volatile ("movl %%cr2, %0;" :"=rm"(cr2) : );
	__asm__ volatile ("movl %%cr3, %0;" :"=rm"(cr3) : );
	__asm__ volatile ("movl %%cr4, %0;" :"=rm"(cr4) : );
	printf("EAX:%x EBX:%x ECX:%x EDX:%x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
	printf("EDI:%x ESI:%x EBP:%x ESP:%x\n", regs->edi, regs->esi, regs->ebp, regs->esp);
	printf("CS:%x SS:%x EFLAGS:%x EIP:%x\n", regs->cs, regs->ss, regs->eflags, regs->eip);
	printf("CR0:%x CR1:00000000 CR2:%x CR3:%x CR4:%x\n\n", cr0, cr2, cr3, cr4);
}

void register_isr(uint8_t interrupt,  void (*func_ptr))
{
	specific_handler[interrupt] = func_ptr;
	handler_exists[interrupt] = 1;
	// if the interrupt is an IRQ, tell the PIC to unmask the line
	if(interrupt > 31 && interrupt < 48)
		pic_unmask_irq(interrupt - 32);
}

void clear_isr(uint8_t interrupt)
{
	specific_handler[interrupt] = 0;
	handler_exists[interrupt] = 0;
	// if the interrupt is an IRQ, tell the PIC to mask the line
	if(interrupt > 31 && interrupt < 48)
		pic_mask_irq(interrupt - 32);
}

void isr_init()
{
	size_t i;
	for(i=0; i<IDT_NUM_ENTRIES; i++)
		clear_isr(i);

	// we tell the IDT to tell us about *all* interrupts
	// we check if we have a handler registered in our table
	// this is because we dont want to hard code ISR addresses
	// and GCC doesn't support writing ISRs in C (we need iret)
	idt_register_isr(0,  &isr0w);
	idt_register_isr(1,  &isr1w);
	idt_register_isr(2,  &isr2w);
	idt_register_isr(3,  &isr3w);
	idt_register_isr(4,  &isr4w);
	idt_register_isr(5,  &isr5w);
	idt_register_isr(6,  &isr6w);
	idt_register_isr(7,  &isr7w);
	idt_register_isr(8,  &isr8w);
	idt_register_isr(10, &isr10w);
	idt_register_isr(11, &isr11w);
	idt_register_isr(12, &isr12w);
	idt_register_isr(13, &isr13w);
	idt_register_isr(14, &isr14w);
	idt_register_isr(16, &isr16w);
	idt_register_isr(17, &isr17w);
	idt_register_isr(18, &isr18w);
	idt_register_isr(19, &isr19w);
	idt_register_isr(20, &isr20w);
	idt_register_isr(30, &isr30w);

	// now the IRQs
	idt_register_isr(32, &isr32w);
	idt_register_isr(33, &isr33w);
	//idt_register_isr(34, &isr34w);
	//idt_register_isr(35, &isr35w);
	//idt_register_isr(36, &isr36w);
	//idt_register_isr(37, &isr37w);
	//idt_register_isr(38, &isr38w);
	//idt_register_isr(39, &isr39w);
	//idt_register_isr(40, &isr40w);
	//idt_register_isr(41, &isr41w);
	//idt_register_isr(42, &isr42w);
	//idt_register_isr(43, &isr43w);
	//idt_register_isr(44, &isr44w);
	//idt_register_isr(45, &isr45w);
	//idt_register_isr(46, &isr46w);
	//idt_register_isr(47, &isr47w);
}
