#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#define PROCESS_RUNNING  0
#define PROCESS_SLEEPING 1
#define PROCESS_STOPPED  2

#include <i386/isr.h>

typedef struct _process_t {
	int pid;
	int state;

	int cpu_time;
	struct _process_t* next_proc;
	
	// machine state
	regs_t regs;
	// TODO: address space things
} process_t;

#endif
