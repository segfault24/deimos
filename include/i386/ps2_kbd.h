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

#ifndef I386_PS2_KBD_H
#define I386_PS2_KBD_H

#include <stdint.h>

// keyboard encoder command codes
#define PS2_KBD_SETLEDS		0xED
#define PS2_KBD_ECHO		0xEE
#define PS2_KBD_SETSCAN		0xF0
#define PS2_KBD_GETID		0xF2
#define PS2_KBD_SETREPEAT	0xF3
#define PS2_KBD_ENABLEKBD	0xF4
#define PS2_KBD_DISABLEKBD	0xF5
#define PS2_KBD_RESET		0xF6
#define PS2_KBD_ALLREPEAT	0xF7
#define PS2_KBD_ALLMAKEBRK	0xF8
#define PS2_KBD_ALLMAKE		0xF9
#define PS2_KBD_ALLALL		0xFA
#define PS2_KBD_SNGLREPEAT	0xFB
#define PS2_KBD_SNGLMAKEBRK	0xFC
#define PS2_KBD_SNGLEBRK	0xFD
#define PS2_KBD_RESEND		0xFE
#define PS2_KBD_RSTTEST		0xFF

void kbd_init();

uint8_t kbd_is_ascii_printable(uint8_t key);
char kbd_key_to_ascii(uint8_t key);

uint8_t kbd_get_scan();
void kbd_clr_scan();
uint8_t kbd_get_key();
void kbd_clr_key();

uint8_t kbd_get_alt();
uint8_t kbd_get_ctrl();
uint8_t kbd_get_shift();
uint8_t kbd_get_numlock();
uint8_t kbd_get_capslock();
uint8_t kbd_get_scrolllock();

#endif


