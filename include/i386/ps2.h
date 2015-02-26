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

#ifndef I386_PS2_H
#define I386_PS2_H

#include <stdint.h>

// controller ports
#define PS2_CMD_PORT	0x64
#define PS2_STATUS_PORT	0x64
#define PS2_DATA_PORT	0x60

// controller status register bits
#define PS2_STATUS_OUTBUFRDY	0x01
#define PS2_STATUS_INBUFFULL	0x02
#define PS2_STATUS_SYSRDY		0x04
#define PS2_STATUS_CMDDATA		0x08
#define PS2_STATUS_UNLOCKED		0x10
#define PS2_STATUS_AUXBUF		0x20
#define PS2_STATUS_TIMEOUT		0x40
#define PS2_STATUS_PARITYERR	0x80

// controller command codes
#define PS2_READCFG		0x20
#define PS2_WRITECFG	0x60
#define PS2_DSBLPORT2	0xA7
#define PS2_ENBLPORT2	0xA8
#define PS2_TESTPORT2	0xA9
#define PS2_SELFTEST	0xAA
#define PS2_TESTPORT1	0xAB
#define PS2_DSBLPORT1	0xAD
#define PS2_ENBLPORT1	0xAE
#define PS2_READIN		0xC0
#define PS2_READOUT		0xD0
#define PS2_WRITEOUT	0xD1
#define PS2_WRITE1OUT	0xD2
#define PS2_WRITE2OUT	0xD3
#define PS2_WRITE2IN	0xD4
#define PS2_READTEST	0xE0
#define PS2_DEVRESET	0xFF

// controller response codes
#define PS2_TESTPASS	0x55

void ps2_init();

// ps/2 devices will need to read the status of the controller
// and the data buffer directly for isrs
uint8_t ps2_read_status();
uint8_t ps2_read_data();

void ps2_write_port1(uint8_t data);
void ps2_write_port2(uint8_t data);

#endif


