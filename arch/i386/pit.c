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

#include <stdint.h>

#include <kernel/sched.h>
#include <kernel/error.h>
#include <i386/asm_util.h>
#include <i386/isr.h>
#include <i386/pit.h>

static unsigned int ticks = 0;

static void tmr_isr()
{
	ticks++;
	do_scheduling(ticks);
}

void pit_init()
{
	// send our configuration
	outb(PIT_CMD, PIT_CH0_CFG);
	// set the reload value
	outb(PIT_CH0_DATA, PIT_CH0_RLDL);
	outb(PIT_CH0_DATA, PIT_CH0_RLDH);
	
	if(request_irq(0, (unsigned int)&ticks, &tmr_isr))
		kpanic("could not register PIT interrupt handler");
}

static void pit_beep_start(uint32_t freq)
{
	// pc speaker timer configuration
	outb(PIT_CMD, PIT_CH2_CFG);
	// set the reload value
	uint32_t reload = 1193182/freq;
	outb(PIT_CH2_DATA, (uint8_t)reload);
	outb(PIT_CH2_DATA, (uint8_t)(reload>>8));
	
	// attach the output of timer 2 to the speaker
	outb(0x61, inb(0x61) | 0x03);
}

static void pit_beep_stop()
{
	// detach the output of timer 2 from the speaker
	outb(0x61, inb(0x61) & !0x03);
}

void beep()
{
	// TODO: add timer, freq and duration selection
	static int on = 0;
	
	if(!on)
	{
		pit_beep_start(2000);
		on = 1;
	}
	else
	{
		pit_beep_stop();
		on = 0;
	}
}
