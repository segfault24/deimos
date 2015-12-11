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

// IO ports for controlling PIT
#define PIT_CH0_DATA 0x40
#define PIT_CH2_DATA 0x42
#define PIT_CMD 0x43

// woo, constants
#define PIT_BASE_FREQ 1193282

// set channel 0 to mode 3 (square wave)
#define PIT_CH0_CFG 0x36

// set channel 2 to mode 3 (square wave)
#define PIT_CH2_CFG 0xB6

static void tmr_isr()
{
	do_scheduling();
}

void pit_init()
{
	// send our configuration
	outb(PIT_CMD, PIT_CH0_CFG);
	
	// calculate and set the reload value
	uint32_t reload = PIT_BASE_FREQ/KERNEL_SCHED_HZ;
	outb(PIT_CH0_DATA, (uint8_t)reload);
	outb(PIT_CH0_DATA, (uint8_t)(reload>>8));
	
	// register the IRQ
	if(request_irq(0, (unsigned int)pit_init, &tmr_isr))
		kpanic("could not register PIT interrupt handler");
}

static void pit_beep_start(uint32_t freq)
{
	// pc speaker timer configuration
	outb(PIT_CMD, PIT_CH2_CFG);
	
	// apply our limits
	if(freq<BEEP_MIN_FREQ)
		freq = BEEP_MIN_FREQ;
	if(freq>BEEP_MAX_FREQ)
		freq = BEEP_MAX_FREQ;
	
	// set the reload value
	uint32_t reload = PIT_BASE_FREQ/freq;
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

void beep(unsigned int freq, unsigned int millis)
{
	pit_beep_start(freq);
	sleep(millis);
	pit_beep_stop();
}
