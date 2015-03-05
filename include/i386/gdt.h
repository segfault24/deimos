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

#ifndef I386_GDT_H
#define I386_GDT_H

#include <stdint.h>

// segment descriptor magic
#define GDT_KERNEL_CODE 0xC09A
#define GDT_KERNEL_DATA 0xC092
#define GDT_USER_CODE   0xC0FA
#define GDT_USER_DATA   0xC0F2

#define GDT_NUM_ENTRIES 5

//                 GDT Entry
// 31                  15                  0
// +-------------------+-------------------+
// |       base        |      limit        |
// |       0:15        |      0:15         |
// +-------------------+-------------------+
//
// 63                  47                 32
// +---------+----+----+---------+---------+
// |  base   |flag|limt| access  |  base   |
// |  24:31  |    |16:19         |  16:23  |
// +---------+----+----+---------+---------+

typedef union
{
	struct
	{
		uint16_t limit;
		uint16_t base_low;
		uint8_t base_mid;
		uint8_t access;
		uint8_t granularity;
		uint8_t base_hi;
	} fields;
	uint64_t bits;
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) gdt_ptr_t;

// sets up the table
void gdt_init();

#endif
