#ifndef KERNEL_SCHED_H
#define KERNEL_SCHED_H

#include <i386/isr.h>

// called by the programmable timer interrupt
// does the main bit of scheduling
void do_scheduling(unsigned int ticks, regs_t* regs);

// uhhhhh, need something to start new processes
int fork();

#endif
