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

#include <stddef.h>
#include <stdint.h>
#include <kernel/stdio.h>
#include <kernel/tty.h>
#include <kernel/error.h>

void kwarn(const char* message)
{
	//tty_setcolor(VGA_YELLOW, VGA_BLACK);
	printf("Warning: %s\n", message);
}

void kerror(const char* message)
{
	//tty_setcolor(VGA_LIGHT_RED, VGA_BLACK);
	printf("Error: %s\n", message);
}

void kpanic(const char* message)
{
	tty_setcolor(VGA_RED, VGA_BLACK);
	printf("Panic: the kernel encountered an unrecoverable error\n");
	printf("Panic: >> %s\n", message);
	printf("Panic: system will now halt...");
	
	for(;;);
}
