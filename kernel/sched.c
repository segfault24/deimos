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

extern page_directory_t* current_pd;

static task_t* cur_task = 0;
int scheduling_enabled = 0;

static void queue_task(task_t* t)
{
	t->next_task = cur_task->next_task;
	t->prev_task = cur_task;
	cur_task->next_task->prev_task = t;
	cur_task->next_task = t;
}

static void dequeue_task(task_t* t)
{
	t->prev_task->next_task = t->next_task;
	t->next_task->prev_task = t->prev_task;
	t->prev_task = 0;
	t->next_task = 0;
}

static task_t* find_task(pid_t pid)
{
	task_t* c = cur_task;
	task_t* t = c->next_task;
	while(t != c)
	{
		if(t->pid == pid)
			return t;
		t = t->next_task;
	}
	return 0;
}

static task_t* get_next_task()
{
	task_t* n;
	task_t* t = cur_task->next_task;
	while(t != cur_task)
	{
		switch(t->state)
		{
			case TASK_READY:
				return t;
			case TASK_DESTROY:
				n = t->prev_task;
				dequeue_task(t);
				free_task(t);
				t = n;
				break;
		}
		t = t->next_task;
	}
	
	// if we somehow find no other task, resume the same task
	return cur_task;
}

static void switch_task()
{
	unsigned int esp, ebp, eip;
	
	disable_interrupts();
	
	// snapshot the current task's registers
	esp = read_esp();
	ebp = read_ebp();
	
	// eip=0x12345 means the task just resumed, so return
	eip = read_eip();
	if(eip == 0x12345)
		return;
	
	// save the current task's registers
	cur_task->eip = eip;
	cur_task->esp = esp;
	cur_task->ebp = ebp;
	
	// the next task becomes the current
	if(cur_task->state != TASK_DESTROY)
		cur_task->state = TASK_READY;
	cur_task = get_next_task();
	cur_task->state = TASK_RUNNING;
	
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
		"r" (current_pd->phys) : "ecx");
	
	// control shouldn't reach here
	kpanic("sched: failed to execute context switch");
}

void sched_init()
{
	// setup the first task and manually queue it to itself
	cur_task = new_task();
	cur_task->state = TASK_RUNNING;
	cur_task->prev_task = cur_task;
	cur_task->next_task = cur_task;
	
	// start scheduling
	scheduling_enabled = 1;
}

void do_scheduling(unsigned int ticks)
{
	ticks++;//dummy
	cur_task->cpu_time++;
	if(scheduling_enabled)
		switch_task();
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

pid_t create_kernel_task(void (*func)(void))
{
	task_t* ktask = new_task();
	ktask->ppid = 0;
	ktask->eip = (unsigned int)func;
	ktask->state = TASK_READY;
	queue_task(ktask);
	
	return ktask->pid;
}

void kill(pid_t pid)
{
	task_t* t = find_task(pid);
	if(!t)
		return;
	t->state = TASK_DESTROY;
}

void sched_yield()
{
	switch_task();
}

void sched_kill()
{
	cur_task->state = TASK_DESTROY;
	switch_task();
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
