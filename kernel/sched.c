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

#include <kernel/process.h>
#include <kernel/sched.h>

//static process_t* cur_proc;

void do_scheduling(unsigned int ticks, regs_t* regs)
{
	ticks++;
	regs++;
	// TODO: this is probably not correct
	//if(0)
	//{
		// store the current regs in the current process' structure
		//cur_proc->regs = *regs;
		
		// load the next processes' regs
		//*regs = cur_proc->next_proc->regs;
		
		// the next process becomes the current
		//cur_proc = cur_proc->next_proc;
	//}
}

int fork()
{
	return 0;
}
