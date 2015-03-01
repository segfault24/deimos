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

static int n_pid = 0;
static int next_pid()
{
	return n_pid++;
}

process_t* new_process()
{
	// allocate space for the process structures
	process_t* proc = kmalloc(sizeof(process_t));
	if(!proc)
		return 0;
	memset(proc, 0, sizeof(process_t));
	proc->pid = next_pid();
	return proc;
}

process_t* clone_process(process_t* parent)
{
	process_t* child = new_process();
	if(!child)
		return 0;
	
	child->parent_pid = parent->pid;
	child->state = PRCOESS_RUNNING;
	
	// setup a page directory
	child->page_dir = get_current_pd();
	
	// setup a stack and the corresponding registers
	child->stack = kmalloc_a(PROCESS_STACK_SIZE);
	if(!child->stack)
	{
		kfree(child);
		return 0;
	}
	
	unsigned int* sp = child->stack + PROCESS_STACK_SIZE;
	child->ebp = (unsigned int)sp;
	
	*--sp = 0xdead; // final return?
	*--sp = 0x202; // EFLAGS
	*--sp = 0x08; // CS
	*--sp = 0xDEAD; // EIP
	*--sp = 0;	// null (err_code)
	*--sp = 0;	// null (int_no)
	*--sp = 0xA;	// EDI
	*--sp = 0xB;	// ESI
	*--sp = 0xC;	// EBP
	*--sp = 0xD;	// null (esp)
	*--sp = 0xE;	// EBX
	*--sp = 0xF;	// EDX
	*--sp = 0x1;	// ECX
	*--sp = 0x2;	// EAX
	*--sp = 0;
	*--sp = 0;
	*--sp = 0;
	*--sp = 0;
	*--sp = 0;
	*--sp = 0;
	
	child->esp = (unsigned int)sp;
	
	return child;
}
