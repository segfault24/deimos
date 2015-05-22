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
#include <kernel/string.h>
#include <kernel/error.h>
#include <kernel/kalloc.h>
#include <i386/asm_util.h>
#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/isr.h>

// wrapper stubs that call our generic interrupt handler
extern void isr0w();  // divide by zero
extern void isr1w();  // debug
extern void isr2w();  // non maskable interrupt
extern void isr3w();  // breakpoint
extern void isr4w();  // overflow
extern void isr5w();  // bound range exceeded
extern void isr6w();  // invalid opcode
extern void isr7w();  // device not available
extern void isr8w();  // double fault
extern void isr10w(); // invalid TSS
extern void isr11w(); // segment not present
extern void isr12w(); // stack segment fault
extern void isr13w(); // general protection fault
extern void isr14w(); // page fault
extern void isr16w(); // x87 floating point exception
extern void isr17w(); // alignment check
extern void isr18w(); // machine check
extern void isr19w(); // SIMD floating point exception
extern void isr20w(); // virtualization exception
extern void isr30w(); // security exception
// 32-47 are technically mapped IRQs
extern void irq0w();  // programmable interrupt timer
extern void irq1w();  // keyboard
// irq2 is for PIC chaining
extern void irq3w();  // COM2
extern void irq4w();  // COM1
extern void irq5w();  // LPT2
extern void irq6w();  // floppy disk
extern void irq7w();  // LPT1/spurious
extern void irq8w();  // CMOS real time clock
extern void irq9w();  // (free)
extern void irq10w(); // (free)
extern void irq11w(); // (free)
extern void irq12w(); // PS/2 mouse
extern void irq13w(); // FPU/coprocessor/inter-processor
extern void irq14w(); // primary ATA hard disk
extern void irq15w(); // secondary ATA hard disk

typedef struct _handler_ptr_t {
	unsigned int id;
	void (*handler)(regs_t* regs);
	struct _handler_ptr_t* next;
} handler_ptr_t;

// Instead of a table of struct pointers, I made it a table of the struct itself
// so we can register our initial isr/irqs before the heap gets initialized.
// Trying to register more than one handler to an interrupt/irq number before the
// heap is initialized will fail.
static handler_ptr_t handlers[256];

// generic interrupt handler for use only by the asm wrappers
void isr_handler(regs_t regs)
{
	// spurious interrupt?
	if(regs.int_no == 39 || regs.int_no == 47)
		if(pic_is_spurious())
			return;
	
	// if the interrupt was an IRQ, tell the PIC we acknowledge it
	// this needs to happen *before* calling the handler so that the
	// timer interrupt can happen again after a context switch
	if(regs.int_no > 31 && regs.int_no < 48)
		pic_send_eoi(regs.int_no - 32);
	
	int i = 0;
	handler_ptr_t* h = &handlers[regs.int_no];
	while(h && h->handler)
	{
		(h->handler)(&regs);
		h = h->next;
		i = 1;
	}
	if(!i)
	{
		dump_regs(&regs);
		kpanic("uncaught interrupt");
	}
}

// TODO: request flags, like exclusive or shared 
int request_isr(unsigned int interrupt, unsigned int id, void(*handler))
{
	if(interrupt > 256)
		return 1;
	
	if(handlers[interrupt].handler == 0)
	{
		// no one else has attached to this interrupt yet
		handlers[interrupt].id = id;
		handlers[interrupt].handler = handler;
		return 0;
	}
	else
	{
		// add it to the chain
		handler_ptr_t* n = kmalloc(sizeof(handler_ptr_t));
		if(!n)
			return 1;
		n->id = id;
		n->handler = handler;
		n->next = 0;
		
		handler_ptr_t* h = &handlers[interrupt];
		while(h->next)
			h = h->next;
		h->next = n;
		return 0;
	}
}

int request_irq(unsigned int irq, unsigned int id, void(*handler))
{
	int ret = request_isr(irq + 32, id, handler);
	if(!ret)
		pic_unmask_irq(irq);
	return ret;
}

void release_isr(unsigned int interrupt, unsigned int id)
{
	handler_ptr_t* h = &handlers[interrupt];
	if(h->id == id)
	{
		if(h->next)
		{
			// move the next one into the table
			h->id = h->next->id;
			h->handler = h->next->handler;
			handler_ptr_t* t = h->next;
			h->next = h->next->next;
			kfree(t);
		}
		else
		{
			// clear the table entry
			memset(h, 0, sizeof(handler_ptr_t));
		}
	}
	else
	{
		while(h->next)
		{
			if(h->next->id == id)
			{
				handler_ptr_t* t = h->next;
				h->next = h->next->next;
				kfree(t);
				return;
			}
			h = h->next;
		}
	}
}

void release_irq(unsigned int irq, unsigned int id)
{
	release_isr(irq + 32, id);
	if(handlers[irq+32].handler == 0)
		pic_mask_irq(irq);
}

void isr_init()
{
	memset(&handlers, 0, 256*sizeof(handler_ptr_t));
	
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
	idt_register_isr(32, &irq0w);
	idt_register_isr(33, &irq1w);
	// irq2 (int34) is for PIC chaining
	idt_register_isr(35, &irq3w);
	idt_register_isr(36, &irq4w);
	idt_register_isr(37, &irq5w);
	idt_register_isr(38, &irq6w);
	idt_register_isr(39, &irq7w);
	idt_register_isr(40, &irq8w);
	idt_register_isr(41, &irq9w);
	idt_register_isr(42, &irq10w);
	idt_register_isr(43, &irq11w);
	idt_register_isr(44, &irq12w);
	idt_register_isr(45, &irq13w);
	idt_register_isr(46, &irq14w);
	idt_register_isr(47, &irq15w);
}

void interrupts_print_info()
{
	size_t i;
	for(i=0; i<256; i++)
	{
		handler_ptr_t* h = &handlers[i];
		while(h && h->handler)
		{
			printf("int:%x\tid:0x%x\thandler:0x%x\n", i, h->id, h->handler);
			h = h->next;
		}
	}
}

void dump_regs(regs_t* regs)
{
	uint32_t cr0, cr2, cr3, cr4;
	
	__asm__ volatile ("movl %%cr0, %0;" :"=rm"(cr0) : );
	__asm__ volatile ("movl %%cr2, %0;" :"=rm"(cr2) : );
	__asm__ volatile ("movl %%cr3, %0;" :"=rm"(cr3) : );
	__asm__ volatile ("movl %%cr4, %0;" :"=rm"(cr4) : );
	printf("\nINT:%x ERR:%x\n", regs->int_no, regs->err_code);
	printf("EAX:%x EBX:%x ECX:%x EDX:%x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
	printf("EIP:%x ESP:%x EBP:%x\n", regs->eip, regs->esp, regs->ebp);
	printf("EDI:%x ESI:%x CS:%x EFLAGS:%x\n", regs->edi, regs->esi, regs->cs, regs->eflags);
	printf("CR0:%x CR1:0 CR2:%x CR3:%x CR4:%x\n", cr0, cr2, cr3, cr4);
}
