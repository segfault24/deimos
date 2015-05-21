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

#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <i386/paging.h>

#define TASK_STOPPED  0
#define TASK_RUNNING  1
#define TASK_SLEEPING 2

#define KERNEL_STACK_SIZE 0x2000
#define STACK_SIZE 0x4000

typedef struct _task_t {
	unsigned int pid, ppid, state, cpu_time;
	page_directory_t* page_dir;
	unsigned int kernel_stack;
	unsigned int eip, esp, ebp;
	
	// only for kernel tasks
	void (*ktask_func)(void);
	
	struct _task_t* next_task;
} task_t;

task_t* new_task();
task_t* clone_task(task_t* parent);
unsigned int create_kernel_task(void (*func)(void));

#endif
