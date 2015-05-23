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
#include <kernel/task.h>
#include <kernel/sched.h>

static unsigned int n_pid = 0;
static unsigned int next_pid()
{
	return n_pid++;
}

static void setup_stack(task_t* t)
{
	// setup a stack and the corresponding registers
	unsigned int* esp = kmalloc_a(STACK_SIZE) + STACK_SIZE;
	unsigned int* stack = esp;
	
	*--stack = 0x202;	// EFLAGS
	*--stack = 0x08;	// CS
	*--stack = t->eip;	// EIP
	
	*--stack = 0;		// EDI
	*--stack = 0;		// ESI
	*--stack = 0;		// EBP
	*--stack = 0;		// null (esp)
	
	*--stack = 0;		// EBX
	*--stack = 0;		// EDX
	*--stack = 0;		// ECX
	*--stack = 0;		// EAX
	
	*--stack = 0x10;	// DS
	*--stack = 0x10;	// ES
	*--stack = 0x10;	// FS
	*--stack = 0x10;	// GS
	
	*--stack = 0;//(unsigned int)&ring3_exit;
	
	t->esp = (unsigned int)stack;
	t->ebp = (unsigned int)esp;
}

void task_print_info(task_t* t)
{
	printf("pid:%u ppid:%u state:%u cpu_time:%u eip:%x esp:%x ebp:%x\n",
		t->pid, t->ppid, t->state, t->cpu_time, t->eip, t->esp, t->ebp);
	//printf("  page_dir:%x kernel_stack:%x\n", t->page_dir, t->kernel_stack);
}

task_t* new_task()
{
	// allocate space for the process structures
	task_t* t = kmalloc(sizeof(task_t));
	
	// process info
	t->pid = next_pid();
	t->ppid = 0;
	t->state = TASK_STOPPED;
	t->cpu_time = 0;
	
	// registers
	t->eip = 0;//(unsigned int)&ring3_exit;
	t->esp = 0;
	t->ebp = 0;
	
	// paging structures
	t->page_dir = get_current_pd();
	
	// stacks
	t->kernel_stack = (unsigned int)kmalloc_a(KERNEL_STACK_SIZE);
	setup_stack(t);
	
	t->prev_task = 0;
	t->next_task = 0;
	
	return t;
}

/*task_t* clone_task(task_t* parent)
{
	task_t* child = new_task();
	
	// set the child's identifiers
	child->ppid = parent->pid;
	
	// free the default stack
	kfree((void*)child->ebp);
	child->ebp = 0;
	child->esp = 0;
	
	// clone the parent's page directory
	child->page_dir = clone_pd(parent->page_dir);
	
	return child;
}*/


