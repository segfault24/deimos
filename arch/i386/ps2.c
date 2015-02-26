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

#include <kernel/error.h>
#include <i386/ioasm.h>
#include <i386/ps2.h>

static void send_cmd(uint8_t cmd)
{
	while(ps2_read_status() & PS2_STATUS_INBUFFULL);
	outb(PS2_CMD_PORT, cmd);
}

static void write_data(uint8_t data)
{
	while(ps2_read_status() & PS2_STATUS_INBUFFULL);
	outb(PS2_DATA_PORT, data);
}

/////////////// TESTS ///////////////
// 1=pass, 0=fail

static uint8_t ps2_test_controller()
{
	send_cmd(PS2_SELFTEST);
	return ps2_read_data() && PS2_TESTPASS;
}

static uint8_t ps2_test_port1()
{
	send_cmd(PS2_TESTPORT1);
	return ps2_read_data() ? 0 : 1;
}

static uint8_t ps2_test_port2()
{
	send_cmd(PS2_TESTPORT2);
	return ps2_read_data() ? 0 : 1;
}

/***********************************/
/*         PUBLIC FUNCTIONS        */
/***********************************/

void ps2_init()
{
	// TODO: initialize usb to prevent interference with ps/2
	// TODO: see if the ps/2 controller actually exists
	// TODO: other configuration

	// disable devices
	send_cmd(PS2_DSBLPORT1);
	send_cmd(PS2_DSBLPORT2);

	// flush the output buffer
	inb(PS2_DATA_PORT);

	// set the controller config byte

	// perform controller self test
	if(!ps2_test_controller())
	{
		kerror("PS/2 controller failed self test");
		return;
	}

	// see if there are two channels

	// test and enable ports, reset attached device
	if(ps2_test_port1())
	{
		send_cmd(PS2_ENBLPORT1);
		ps2_write_port1(PS2_DEVRESET);
	}
	if(ps2_test_port2())
	{
		send_cmd(PS2_ENBLPORT2);
		ps2_write_port2(PS2_DEVRESET);
	}
}

uint8_t ps2_read_status()
{
	return inb(PS2_STATUS_PORT);
}

uint8_t ps2_read_data()
{
	while(ps2_read_status() & !PS2_STATUS_OUTBUFRDY);
	return inb(PS2_DATA_PORT);
}

void ps2_write_port1(uint8_t data)
{
	write_data(data);
}

void ps2_write_port2(uint8_t data)
{
	send_cmd(PS2_WRITE2IN);
	write_data(data);
}
