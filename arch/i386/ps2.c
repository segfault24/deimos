#include <stddef.h>
#include <stdint.h>
#include <i386/isr.h>
#include <i386/ioasm.h>
#include <i386/ps2_codes.h>
#include <i386/ps2_keys.h>
#include <i386/ps2.h>

static uint8_t ctrl = 0;
static uint8_t alt = 0;
static uint8_t shift = 0;
static uint8_t numlock = 0;
static uint8_t capslock = 0;
static uint8_t scrolllock = 0;
static uint8_t buf = 0;;

static uint8_t kbd_ctrl_read_status()
{
	return inb(PS2_KBD_CTRL_STATUS_REG);
}

static void kbd_ctrl_send_cmd(uint8_t cmd)
{
	while(kbd_ctrl_read_status() & PS2_KBD_CTRL_INBUFFULL);
	outb(PS2_KBD_CTRL_CMD_REG, cmd);
}

static uint8_t kbd_enc_read_buf()
{
	return inb(PS2_KBD_ENC_DATA_BUF);
}

static void kbd_enc_send_cmd(uint8_t cmd)
{
	while(kbd_ctrl_read_status() & PS2_KBD_CTRL_INBUFFULL);
	outb(PS2_KBD_ENC_CMD_REG, cmd);
}

static void kbd_update_leds()
{
	uint8_t data = 0;
	data = scrolllock ? (data|1) : data ;
	data = numlock ? (data|2) : data;
	data = capslock ? (data|4) : data;
	kbd_enc_send_cmd(PS2_KBD_ENC_SETLEDS);
	kbd_enc_send_cmd(data);
}

static void kbd_isr()
{
	uint8_t scan_code;
	uint16_t key;

	// if the buffer isnt ready, return
	if(!(kbd_ctrl_read_status() & PS2_KBD_CTRL_OUTBUFRDY))
		return;

	// read the controller buffer
	scan_code = kbd_enc_read_buf();

	// TODO: check for errors
	//switch(scan_code)
	//{
	//	case bat_failed: bat = false; return;
	//	case diag_failed: diag = false; return;
	//	case resend_cmd: resend = true; return;
	//}

	// check for extended
	if(scan_code == 0xE0 || scan_code == 0xE1)
		return;

	// if control has reached here, it must be the second
	// byte of an extended or just a single
	if(scan_code & 0x80) // is it a break code?
	{
		key = xt_scan[scan_code-0x80];
		switch(key) // check if it is special
		{
			case KEY_LCTRL:
			case KEY_RCTRL:
				ctrl = 0;
				break;
			case KEY_LSHIFT:
			case KEY_RSHIFT:
				shift = 0;
				break;
			case KEY_LALT:
			case KEY_RALT:
				alt = 0;
				break;
		}
	} else { // otherwise it must be a make code
		buf = scan_code;
 		key = xt_scan[scan_code];
		switch(key) // check if it is special
		{
			case KEY_LCTRL:
			case KEY_RCTRL:
				ctrl = 1;
				break;
			case KEY_LSHIFT:
			case KEY_RSHIFT:
				shift = 1;
				break;
			case KEY_LALT:
			case KEY_RALT:
				alt = 1;
				break;
			case KEY_CAPSLOCK:
				capslock = capslock ? 0 : 1;
				kbd_update_leds();
				break;
			case KEY_NUMLOCK:
				numlock = numlock ? 0 : 1;
				kbd_update_leds();
				break;
			case KEY_SCROLLLOCK:
				scrolllock = scrolllock ? 0 : 1;
				kbd_update_leds();
				break;
		}
	}
}

/***********************************/
/*         PUBLIC FUNCTIONS        */
/***********************************/

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

// TODO: all of the below

uint8_t ps2_test()
{
	return 0;
}

void ps2_reset()
{
	kbd_ctrl_send_cmd(PS2_KBD_CTRL_WRITEOUT);
	kbd_enc_send_cmd(PS2_KBD_ENC_RESEND);
}

uint8_t ps2_is_ascii_printable(uint8_t key)
{
	return ((key>=0x20 && key<=0x7E) || key=='\n' || key=='\t' || key=='\b') ? 1 : 0;
}

char ps2_key_to_ascii(uint8_t key)
{
	if(!ps2_is_ascii_printable(key))
		return 0;

	if((shift || capslock) && key>='a' && key<='z')
		return (key-32);

	if(shift && !capslock)
	{
		switch(key)
		{
			case '1': key = KEY_EXCLAMATION; break;
			case '2': key = KEY_AT; break;
			case '3': key = KEY_HASH; break;
			case '4': key = KEY_DOLLAR; break;
			case '5': key = KEY_PERCENT; break;
			case '6': key = KEY_CARRET; break;
			case '7': key = KEY_AMPERSAND; break;
			case '8': key = KEY_ASTERISK; break;
			case '9': key = KEY_LEFTPARENTHESIS; break;
			case '0': key = KEY_RIGHTPARENTHESIS; break;
			case '`': key = KEY_TILDE; break;
			case '-': key = KEY_UNDERSCORE; break;
			case '=': key = KEY_PLUS; break;
			case '[': key = KEY_LEFTCURL; break;
			case ']': key = KEY_RIGHTCURL; break;
			case '\\': key = KEY_BAR; break;
			case ';': key = KEY_COLON; break;
			case '\'': key = KEY_DOUBLEQUOTE; break;
			case ',': key = KEY_LESS; break;
			case '.': key = KEY_GREATER; break;
			case '/': key = KEY_QUESTION; break;
		}
	}
	return key;
}

uint8_t ps2_get_scan() { return buf; }
void ps2_clr_scan() { buf = 0; }
uint8_t ps2_get_key() { return (uint8_t)xt_scan[buf]; }
void ps2_clr_key() { buf = 0; }

uint8_t ps2_get_alt() { return alt; }
uint8_t ps2_get_ctrl() { return ctrl; }
uint8_t ps2_get_shift() { return shift; }
uint8_t ps2_get_numlock() { return numlock; }
uint8_t ps2_get_capslock() { return capslock; }
uint8_t ps2_get_scrolllock() { return scrolllock; }

