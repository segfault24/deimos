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

#include <kernel/arch.h>

#include <i386/asm_util.h>
#include <i386/gdt.h>
#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/isr.h>
#include <i386/pit.h>
#include <i386/ps2.h>
#include <i386/ps2_kbd.h>

void arch_init()
{
	gdt_init(); // setup the global descriptor table
	idt_init(); // setup the interrupt descriptor table
	pic_init(); // must do this before isr_init()
	isr_init(); // setup the various components of the interrupt system
	
	pit_init(); // setup the interrupt timer for a sysclock
	ps2_init(); // setup the ps/2 controller(s)
	kbd_init(); // setup the ps/2 keyboard
	
	enable_interrupts(); // now we're ready
}
