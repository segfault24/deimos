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

#ifndef I386_IOASM_H
#define I386_IOASM_H

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

#endif
