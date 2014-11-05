#include <stddef.h>
#include <stdint.h>
#include <i386/isr.h>
#include <i386/ioasm.h>
#include <i386/ps2.h>

static char us_layout[128] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
	'm', ',', '.', '/',  0, '*',  0, ' ', 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char kbd_buf = '\0';

// TODO: make this safer
static void kbd_isr()
{
	uint8_t scancode = inb(PS2_REG_DATA);
	if(scancode & 0x80) // check if key release
	{
		kbd_buf = us_layout[scancode-128];
	}
}

char kbd_getchar()
{
	return kbd_buf;
}

void kbd_clearchar()
{
	kbd_buf = '\0';
}

void ps2_init()
{
	isr_register_isr(33, &kbd_isr);

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
