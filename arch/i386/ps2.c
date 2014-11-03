#include <i386/stddef.h>
#include <i386/stdint.h>
#include <i386/ioasm.h>
#include <i386/ps2.h>

void ps2_init()
{
	// initialize usb controllers to prevent interference
	// see if the ps/2 controller actually exists

	// disable devices
	//outb(PS2_REG_CMD, 0xAD);
	//outb(PS2_REG_CMD, 0xA7);

	// flush the output buffer
	//inb(PS2_RED_DATA);

	// set the controller config byte

	// perform controller self test
	//outb(PS2_REG_CMD, 0xAA);
	//inb(PS2_REG_CMD) == 0x55;

	// see if there are two channels

	// perform interface test

	// enable devices
	//outb(PS2_REG_CMD, 0xAE); // 1st port
	//outb(PS2_REG_CMD, 0xA8); // 2nd port

	// reset devices
	//   - send 0xFF to each device
	//   - 0xFA success, 0xFC failure
}
