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

#ifndef I386_IDT_H
#define I386_IDT_H

#include <stdint.h>

// interrupt descriptor magic
#define IDT_KERNEL_INT  0x8E
#define IDT_KERNEL_TRAP 0x8F
#define IDT_KERNEL_NULL 0x00

#define IDT_NUM_ENTRIES 256

//                 IDT Entry
// 31                  15                  0
// +-------------------+-------------------+
// |      selector     |      offset       |
// |       0:15        |       0:15        |
// +-------------------+-------------------+
//
// 63                  47                 32
// +-------------------+-+--+----+---------+
// |      offset       |P|DP|type|    0    |
// |       16:31       | |L |    |         |
// +-------------------+-+--+----+---------+

typedef union
{
	struct
	{
		uint16_t offset_low;
		uint16_t selector;
		uint8_t zero;
		uint8_t types;
		uint16_t offset_hi;
	} fields;
	uint64_t bits;
} __attribute__((packed)) idt_entry_t;


typedef struct
{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) idt_ptr_t;

idt_ptr_t iptr;

// registers an isr in the idt
void idt_register_isr(uint8_t interrupt, void (*func_ptr));
// clears an isr in the idt
void idt_clear_isr(uint8_t interrupt);

// sets up the table
// this does not register any ISRs
void idt_init();

// calls the lidt asm instruction
extern void idt_load();

#endif
