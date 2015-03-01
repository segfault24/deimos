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

static void switch_task()
{
	int esp;//, ebp;//, eip;
	
	if(!cur_proc)
		return;
	
	// snapshot the current task's registers
	__asm__ volatile ( "mov %%esp, %0" : "=r" (esp) );
	//__asm__ volatile ( "mov %%ebp, %0" : "=r" (ebp) );
	
	//eip = read_eip();
	//if(eip == 0x12345)
	//	return;
	
	// save the current task's registers
	//cur_proc->eip = eip;
	cur_proc->esp = esp;
	//cur_proc->ebp = ebp;
	
	// the next process becomes the current
	cur_proc = cur_proc->next_proc;
	
	// load the next task's registers
	//eip = cur_proc->eip;
	esp = cur_proc->esp;
	//ebp = cur_proc->ebp;
	
	// load the next process' page directory
	switch_directory(cur_proc->page_dir);
	
	/*__asm__ volatile ( "		\
		mov %0, %%ecx;			\
		mov %1, %%esp;			\
		mov %2, %%ebp;			\
		mov $0x12345, %%eax;	\
		jmp *%%ecx				\
		" : : "r" (eip), "r" (esp), "r" (ebp));*/
	
	__asm__ volatile ( "mov %0, %%esp" : : "r" (esp) );
	//__asm__ volatile ( "mov %0, %%ebp" : : "r" (ebp) );
}

void sched_init()
{
	// setup the first process and queue (to itself)
	process_t* p = new_process();
	if(!p)
		kpanic("could not allocate memory for kernel process struct");
	p->state = PROCESS_RUNNING;
	p->page_dir = get_kernel_pd();
	p->next_proc = p;
	
	cur_proc = p;
}

void do_scheduling(unsigned int ticks)
{
	ticks++;//dummy
	if(0)
		switch_task();
}

int kfork()
{
	process_t* child;
	process_t* parent;
	
	disable_interrupts();
	
	parent = cur_proc;
	
	// allocate a new process struct
	child = clone_process(parent);
	if(!child)
		kpanic("kfork: could not allocate memory for child process structures");
	
	// both parent & child will start from here
	//int eip = read_eip();
	if(cur_proc == parent)
	{
		// set child's pointers
		//__asm__ volatile ( "mov %%esp, %0" : "=r" (child->esp));
		//__asm__ volatile ( "mov %%ebp, %0" : "=r" (child->ebp));
		//child->eip = eip;
		
		// insert the new process into the queue
		child->next_proc = parent->next_proc;
		parent->next_proc = child;
		
		enable_interrupts();
		return child->pid;
	}
	else
	{
		return 0;
	}
}
