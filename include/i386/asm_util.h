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
 
#ifndef I386_ASM_UTIL_H
#define I386_ASM_UTIL_H

#include <stdint.h>

static inline void iowait()
{
	__asm__ volatile ( "outb %%al, $0x80" : : "a"(0) );
}

static inline void outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
	return ret;
}

static inline void outw(uint16_t port, uint16_t val)
{
	__asm__ volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm__ volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
	return ret;
}

static inline void outl(uint16_t port, uint32_t val)
{
	__asm__ volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint32_t inl(uint16_t port)
{
	uint32_t ret;
	__asm__ volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
	return ret;
}

static inline void enable_interrupts()
{
	__asm__ volatile ( "sti;nop" );
}
static inline void disable_interrupts()
{
	__asm__ volatile ( "cli;nop" );
}

static inline void halt()
{
	__asm__ volatile ( "hlt" );
}

extern void gdt_load();
extern void idt_load();
extern unsigned int read_eip();

#define read_esp() \
((unsigned int)({\
	unsigned int esp;\
	__asm__ volatile ( "mov %%esp, %0" : "=r"(esp) );\
	esp;\
}))

#define read_ebp() \
((unsigned int)({\
	unsigned int ebp;\
	__asm__ volatile ( "mov %%ebp, %0" : "=r"(ebp) );\
	ebp;\
}))

static inline unsigned int read_cr0()
{
	unsigned int cr0;
	__asm__ volatile ( "movl %%cr0, %0" : "=rm"(cr0) );
	return cr0;
}

static inline unsigned int read_cr2()
{
	unsigned int cr2;
	__asm__ volatile ( "movl %%cr2, %0" : "=rm"(cr2) );
	return cr2;
}

static inline unsigned int read_cr3()
{
	unsigned int cr3;
	__asm__ volatile ( "movl %%cr3, %0" : "=rm"(cr3) );
	return cr3;
}

static inline void write_cr3(unsigned int cr3)
{
	__asm__ volatile ( "movl %0, %%cr3" : : "r"((uint32_t)cr3) );
}

static inline unsigned int read_cr4()
{
	unsigned int cr4;
	__asm__ volatile ( "movl %%cr4, %0" : "=rm"(cr4) );
	return cr4;
}

#endif
