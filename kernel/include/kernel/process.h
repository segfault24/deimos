#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#define PROCESS_RUNNING  0
#define PROCESS_SLEEPING 1
#define PROCESS_STOPPED  2

typedef struct process {
	int pid;
	char state;
	/* registers */
	/* virtual memory */
} process_t;

#endif
