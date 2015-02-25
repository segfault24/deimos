#include <stdint.h>

#include <kernel/sched.h>
#include <i386/ioasm.h>
#include <i386/isr.h>
#include <i386/pit.h>

static unsigned int ticks = 0;

static void tmr_isr(regs_t* regs)
{
	ticks++;
	do_scheduling(ticks, regs);
}

void pit_init()
{
	// send our configuration
	outb(PIT_CMD, PIT_CH0_CFG);
	// set the reload value
	outb(PIT_CH0_DATA, PIT_CH0_RLDL);
	outb(PIT_CH0_DATA, PIT_CH0_RLDH);

	isr_register_isr(32, &tmr_isr);
}
