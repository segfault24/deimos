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

#ifndef I386_PIT_H
#define I386_PIT_H

#define PIT_CH0_DATA 0x40
#define PIT_CH2_DATA 0x42
#define PIT_CMD 0x43

// set channel 0 to mode 3 (square wave)
#define PIT_CH0_CFG 0x36
// with a reload value of 0xFFFF
// which is a rate of 18.2065Hz
#define PIT_CH0_RLDH 0xFF
#define PIT_CH0_RLDL 0xFF

//void pit_beep();

void pit_init();

#endif
