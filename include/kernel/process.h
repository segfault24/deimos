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

#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <i386/isr.h>
#include <i386/paging.h>

#define PROCESS_RUNNING  0
#define PROCESS_SLEEPING 1
#define PROCESS_STOPPED  2

#define PROCESS_STACK_SIZE 0x4000

typedef struct _process_t {
	int pid;
	int parent_pid;
	int state;
	int cpu_time;
	
	page_directory_t* page_dir;
	void* stack;
	unsigned int eip, esp, ebp;
	
	struct _process_t* next_proc;
} process_t;

process_t* new_process();
process_t* clone_process(process_t* parent);

#endif
