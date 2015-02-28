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

#include <kernel/stdio.h>
#include <kernel/string.h>
#include <kernel/error.h>
#include <i386/paging.h>
#include <kernel/kalloc.h>
#include <kernel/process.h>
#include <kernel/sched.h>

extern unsigned int read_eip();

static process_t* cur_proc = 0;
static int n_pid = 0;

static int next_pid()
{
	return n_pid++;
}

// returns a blank process
static process_t* new_process()
{
	process_t* proc;
	regs_t* regs;
	
	// allocate space for the process structures
	proc = kmalloc(sizeof(process_t));
	if(!proc)
		return 0;
	regs = kmalloc(sizeof(regs_t));
	if(!regs)
	{
		kfree(proc);
		return 0;
	}
	
	proc->pid = next_pid();
	proc->parent_pid = 0;
	proc->state = PROCESS_RUNNING;
	proc->cpu_time = 0;
	memset(regs, 0, sizeof(regs_t));
	proc->regs = regs;
	
	return proc;
}

static void switch_task(regs_t* regs)
{
	dump_regs(regs);
	return;
	
	if(!cur_proc)
		return;
	
	// store the current regs in the current process' structure
	*(cur_proc->regs) = *regs;
	
	// the next process becomes the current
	cur_proc = cur_proc->next_proc;
	
	// load the next process' regs
	//*regs = *(cur_proc-->regs);
	
	// next task's pushal regs put on the current stack
	regs->eax = cur_proc->regs->eax;
	regs->ebx = cur_proc->regs->ebx;
	regs->ecx = cur_proc->regs->ecx;
	regs->edx = cur_proc->regs->edx;
	regs->esi = cur_proc->regs->esi;
	regs->edi = cur_proc->regs->edi;
	regs->ebp = cur_proc->regs->ebp;
	regs->esp = cur_proc->regs->esp;
	
	// load the next process' page directory
	switch_directory(cur_proc->page_dir);
}

void sched_init()
{
	// setup the first process and queue (to itself)
	process_t* p = new_process();
	if(!p)
		kpanic("could not allocate memory for kernel process struct");
	p->page_dir = get_kernel_pd();
	p->next_proc = p;
	
	cur_proc = p;
}

void do_scheduling(unsigned int ticks, regs_t* regs)
{
	ticks++;//dummy
	switch_task(regs);
}

int kfork()
{
	process_t* child;
	process_t* parent;
	
	disable_interrupts();
	
	parent = cur_proc;
	
	// allocate a new process struct
	child = new_process();
	if(!child)
		kpanic("kfork: could not allocate memory for process struct");
	child->parent_pid = parent->pid;
	
	// TODO: this is wrong
	// setup a page directory
	child->page_dir = get_kernel_pd();
	if(!child->page_dir)
		kpanic("kfork: could not allocate memory for process page directory");
	
	// setup a stack and the corresponding registers
	child->stack = kmalloc_a(PROCESS_STACK_SIZE);
	if(!child->stack)
		kpanic("kfork: could not allocate memory for process stack");
	
	unsigned int* sp = child->stack + PROCESS_STACK_SIZE;
	child->regs->ebp = (unsigned int)sp;
	
	*--sp = 0x00; // SS
	*--sp = 0; // USERESP
	*--sp = 0x0202; // EFLAGS
	*--sp = 0x08; // CS
	*--sp = parent->regs->eip; // EIP
	
	*--sp = 0; // ERR_CODE
	*--sp = 0; // INT_NO
	
	*--sp = 0; // EAX
	*--sp = 0; // ECX
	*--sp = 0; // EDX
	*--sp = 0; // EBX
	*--sp = 0; // ESP
	*--sp = 0; // EBP
	*--sp = 0; // ESI
	*--sp = 0; // EDI
	
	child->regs->esp = (unsigned int)sp;
	
	// insert the new process into the queue
	child->next_proc = parent->next_proc;
	parent->next_proc = child;
	
	// both parent & child will start from here
	int eip = read_eip();
	if(cur_proc == parent)
	{
		// set child's eip
		child->regs->eip = eip;
		
		enable_interrupts();
		return child->pid;
	}
	else
	{
		return 0;
	}
}
