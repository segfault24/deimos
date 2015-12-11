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

static task_t* ready_queue_head = 0;
static task_t* ready_queue_tail = 0;
static task_t* sleep_queue_head = 0;
//static task_t* sleep_queue_tail = 0;

static task_t* running_task = 0;
static int scheduling_enabled = 0;
static unsigned int ticks = 0;

// 
static task_t* find_task(pid_t pid)
{
	task_t* t;
	
	// check running
	if(running_task->pid == pid)
		return running_task;
	
	// check ready queue
	t = ready_queue_head;
	while(t)
	{
		if(t->pid == pid)
			return t;
		t = t->next_task;
	}
	
	// check sleep queue
	t = sleep_queue_head;
	while(t)
	{
		if(t->pid == pid)
			return t;
		t = t->next_task;
	}
	
	return 0;
}

// 
static void enqueue_ready_task(task_t* t)
{
	if(!ready_queue_head && !ready_queue_tail)
	{
		ready_queue_head = t;
		ready_queue_tail = t;
		return;
	}
	else
	{
		ready_queue_tail->next_task = t;
		ready_queue_tail = t;
	}
}

// 
static task_t* get_next_ready_task()
{
	task_t* t = ready_queue_head;
	
	if(!ready_queue_head)
	{
		kpanic("sched: ready queue should never be empty!");
	}
	
	if(ready_queue_head == ready_queue_tail)
	{
		ready_queue_head = 0;
		ready_queue_tail = 0;
	}
	else
	{
		ready_queue_head = ready_queue_head->next_task;
	}
	
	t->next_task = 0;
	return t;
}

// the task's sleep_ticks should be it's total time to sleep
// so we need to find where in the queue it should go and change
// the sleep ticks to it's delta value
/*static void enqueue_sleep_task(task_t* t)
{
	t++;//dummy
}*/

// returns head of sleep queue if its ready to run
// basically, call this until it returns 0
/*static task_t* dequeue_sleep_task()
{
	task_t* t = sleep_queue_head;
	
	if(t && t->sleep_ticks == 0)
	{
		sleep_queue_head = t->next_task;
		if(!sleep_queue_head)
			sleep_queue_tail = 0;
		
		t->next_task = 0;
		return t;
	}
	else
	{
		return 0;
	}
}*/

// updates head tick delta and moves ready tasks to ready queue
/*static void update_sleep_queue()
{
	task_t* t;
	
	// decrement the sleep queue head's delta
	if(sleep_queue_head)
		sleep_queue_head->sleep_ticks--;
	
	// process the sleep queue
	do
	{
		t = dequeue_sleep_task();
		if(!t)
		{
			// task is ready for waking up
			t->state = TASK_READY;
			enqueue_ready_task(t);
		}
	} while(t);
}*/

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
	running_task->eip = eip;
	running_task->esp = esp;
	running_task->ebp = ebp;
	
	//////////////////////////////////////////
	
	// what do we do with the current task?
	switch(running_task->state)
	{
		case TASK_RUNNING:
			running_task->state = TASK_READY;
			enqueue_ready_task(running_task);
			break;
		case TASK_DESTROY:
			// it will be destroyed when it comes to the head of the queue
			enqueue_ready_task(running_task);
			break;
		case TASK_SLEEPING:
			// this task was just put to sleep
			// its current sleep_ticks is total, not delta
			//enqueue_sleep_task(running_task);
			break;
		default:
			kpanic("sched: how did you reach here???");
			break;
	}
	
	// the next ready task becomes the current, and we
	// destroy any tasks marked for destruction as we go
	task_t* t;
	running_task = 0;
	do
	{
		t = get_next_ready_task();
		if(t->state == TASK_DESTROY)
			free_task(t);
		else
			running_task = t;
	} while(!running_task);
	running_task->state = TASK_RUNNING;
	
	//////////////////////////////////////////
	
	// load the next task's registers
	eip = running_task->eip;
	esp = running_task->esp;
	ebp = running_task->ebp;
	
	// load the next process' page directory
	current_pd = running_task->page_dir;
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

static inline void print_task(task_t* t)
{
	printf("pid:%u ppid:%u state:%u cpu_time:%u eip:%x esp:%x ebp:%x\n",
		t->pid, t->ppid, t->state, t->cpu_time, t->eip, t->esp, t->ebp);
	//printf("  page_dir:%x kernel_stack:%x\n", t->page_dir, t->kernel_stack);
}

void sched_init()
{
	// setup the first task (this becomes the idle task)
	running_task = new_task();
	running_task->state = TASK_RUNNING;
	running_task->next_task = 0;
	
	// start scheduling
	scheduling_enabled = 1;
}

void sched_print_info()
{
	task_t* t;
	
	if(scheduling_enabled)
		printf("scheduler is running\n");
	else
		printf("scheduler is not running\n");
	
	// dump the running task
	print_task(running_task);
	
	// dump the ready queue
	t = ready_queue_head;
	while(t && t!=running_task)
	{
		print_task(t);
		t = t->next_task;
	}
	
	// dump the sleep queue
	t = sleep_queue_head;
	while(t)
	{
		print_task(t);
		t = t->next_task;
	}
}

// called by timer isr only
// ticks and sleep queue stuff must be done here since
// switch_task() can be called by sched_yield()
void do_scheduling()
{
	ticks++;
	
	if(scheduling_enabled)
	{
		// update current task's cpu time counter
		running_task->cpu_time++;
		
		// process the sleep queue
		//update_sleep_queue();
		
		// go into main scheduling routine
		switch_task();
	}
}

pid_t create_kernel_task(void (*func)(void))
{
	task_t* ktask;
	
	ktask = new_task();
	if(!ktask)
	{
		kerror("sched: could not create new kernel task");
		return 0;
	}
	ktask->ppid = 0;
	ktask->eip = (unsigned int)func;
	ktask->state = TASK_READY;
	
	// put it on the ready queue
	enqueue_ready_task(ktask);
	
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
	running_task->state = TASK_DESTROY;
	switch_task();
}

// TODO: SMP will fuck up the sleep queue design
// TODO: actual POSIX return behavior on sleep functions
unsigned int sleep(unsigned int seconds)
{
	running_task->sleep_ticks = KERNEL_SCHED_HZ*seconds;
	running_task->state = TASK_SLEEPING;
	switch_task();
	
	// TODO: need to re-enable interrupts?
	
	return 0;//dummy
}

int usleep(unsigned int millis)
{
	running_task->sleep_ticks = (KERNEL_SCHED_HZ*millis)/1000;
	running_task->state = TASK_SLEEPING;
	switch_task();
	
	// TODO: need to re-enable interrupts?
	
	return 0;//dummy
}

/////////////////////////////////////////////////////////////////////////////////////

/*int kfork()
{
	task_t* child;
	task_t* parent;
	
	disable_interrupts();
	
	parent = running_task;
	
	// allocate a new task struct
	child = clone_task(parent);
	if(!child)
		kpanic("kfork: could not allocate memory for child task structures");
	
	// both parent & child will start from here
	//int eip = read_eip();
	if(running_task == parent)
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
