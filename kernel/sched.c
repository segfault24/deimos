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
#include <i386/asm_util.h>
#include <i386/paging.h>
#include <kernel/kalloc.h>
#include <kernel/task.h>
#include <kernel/sched.h>

extern unsigned int read_eip();
extern page_directory_t* current_pd;

unsigned int idle_pid = 0;
task_t* cur_task = 0;
int scheduling_enabled = 0;

static void switch_task()
{
	int esp, ebp, eip;
	
	if(!cur_task)
		return;
	
	disable_interrupts();
	
	// snapshot the current task's registers
	esp = read_esp();
	ebp = read_ebp();
	
	eip = read_eip();
	if(eip == 0x12345)
		return;
	
	// save the current task's registers
	cur_task->eip = eip;
	cur_task->esp = esp;
	cur_task->ebp = ebp;
	
	printf("\nsave: ");task_print_info(cur_task);
	
	// the next task becomes the current
	cur_task = cur_task->next_task;
	
	printf("new: ");task_print_info(cur_task);
	
	// load the next task's registers
	eip = cur_task->eip;
	esp = cur_task->esp;
	ebp = cur_task->ebp;
	
	// load the next process' page directory
	current_pd = cur_task->page_dir;
	//switch_kernel_stack();
	
	// do the actual switch
	__asm__ volatile ( "		\
		mov %0, %%ecx;			\
		mov %1, %%esp;			\
		mov %2, %%ebp;			\
		mov %3, %%cr3;			\
		mov $0x12345, %%eax;	\
		sti;					\
		nop;					\
		jmp *%%ecx"
		: : "r" (eip), "r" (esp), "r" (ebp),
		"r" (current_pd->phys));
	
	// control shouldn't reach here
	kpanic("sched: failed to execute context switch");
}

static void idle_loop()
{
	unsigned int i = 0;
	while(1)
		if(i++%1000000 == 0)
			putchar('i');
}

void queue_task(task_t* t)
{
	t->next_task = cur_task->next_task;
	cur_task->next_task = t;
}

void sched_init()
{
	// setup the first task and manually queue it to itself
	cur_task = new_task();
	cur_task->next_task = cur_task;
	
	// start the idle task
	idle_pid = create_kernel_task(idle_loop);
	
	// start scheduling
	scheduling_enabled = 0;
}

void do_scheduling(unsigned int ticks)
{
	ticks++;//dummy
	if(scheduling_enabled)
		switch_task();
}

unsigned int get_idle_pid()
{
	return idle_pid;
}

void sched_print_info()
{
	task_t* t = cur_task;
	task_print_info(t);
	t = t->next_task;
	while(t != cur_task)
	{
		task_print_info(t);
		t = t->next_task;
	}
}

/////////////////////////////////////////////////////////////////////////////////////

/*int kfork()
{
	task_t* child;
	task_t* parent;
	
	disable_interrupts();
	
	parent = cur_task;
	
	// allocate a new task struct
	child = clone_task(parent);
	if(!child)
		kpanic("kfork: could not allocate memory for child task structures");
	
	// both parent & child will start from here
	//int eip = read_eip();
	if(cur_task == parent)
	{
		// set child's pointers
		//child->esp = read_esp();
		//child->ebp = read_ebp();
		//child->eip = eip;
		
		// insert the new process into the queue
		child->next_task = parent->next_task;
		parent->next_task = child;
		
		enable_interrupts();
		return child->pid;
	}
	
	return 0;
}*/
