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
