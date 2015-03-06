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

#ifndef I386_ISR_H
#define I386_ISR_H

#include <stdint.h>

typedef struct _regs_t {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // from pushal
    unsigned int int_no, err_code; // from our isr/irq wrappers
    unsigned int eip, cs, eflags; // cpu automatically pushes these
} regs_t;

// Registered ISRs should be in the form "void fname(uint32_t error)".
// These ISRs should be made as short as possible.
// Error numbers have no meaning for IRQs, and can be discarded.
/*void register_isr(uint8_t interrupt, void (*func_ptr));
void clear_isr(uint8_t interrupt);*/

int request_isr(unsigned int interrupt, unsigned int id, void(*handler));
int request_irq(unsigned int irq, unsigned int id, void(*handler));
void release_isr(unsigned int interrupt, unsigned int id);
void release_irq(unsigned int irq, unsigned int id);

void enable_interrupts();
void disable_interrupts();

void dump_regs(regs_t* regs);

void isr_init();
void interrupts_print_info();

#endif
