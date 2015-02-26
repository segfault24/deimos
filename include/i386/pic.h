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

#ifndef I386_PIC_H
#define I386_PIC_H

#include <stdint.h>

#define PIC_MASTER_CMD  0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD   0xA0
#define PIC_SLAVE_DATA  0xA1

#define PIC_INIT 0x11
#define PIC_8086 0x01
#define PIC_EOI  0x20

#define PIC_REG_IRR 0x0A
#define PIC_REG_ISR 0x0B

// maps IRQs 0-7 to interrupts 32-39
#define PIC_MASTER_OFFSET 0x20
// maps IRQs 8-15 to interrupts 40-47
#define PIC_SLAVE_OFFSET 0x28

#define PIC_MASTER_CSCD 0x04
#define PIC_SLAVE_CSCD 0x02

// mask all IRQs by default
#define PIC_MASTER_MASK 0xFF
#define PIC_SLAVE_MASK 0xFF

// APIC functions
// extern void pic_apic_present();
// void pic_disable();

// initializes the PICs and then remaps
// the IRQs so we don't hide CPU exceptions
void pic_init();

// sets the masks on the individual IRQ lines
void pic_mask_irq(uint8_t irq);
void pic_unmask_irq(uint8_t irq);
uint8_t pic_read_mask();

// allows us to read the PICs' internal in-service
// and interrupt request registers
uint16_t pic_read_reg(uint8_t pic_reg);

// tell the PICs we're done
void pic_send_eoi(uint8_t irq);

#endif
