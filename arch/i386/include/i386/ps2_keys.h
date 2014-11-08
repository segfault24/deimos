#ifndef I386_PS2_KEYS_H
#define I386_PS2_KEYS_H

// these defines are used to define scan code sets
// values are either ascii or our own code (for non ascii)

/** BEGIN ASCII KEYS **/

#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39

#define KEY_A 0x61
#define KEY_B 0x62
#define KEY_C 0x63
#define KEY_D 0x64
#define KEY_E 0x65
#define KEY_F 0x66
#define KEY_G 0x67
#define KEY_H 0x68
#define KEY_I 0x69
#define KEY_J 0x6A
#define KEY_K 0x6B
#define KEY_L 0x6C
#define KEY_M 0x6D
#define KEY_N 0x6E
#define KEY_O 0x6F
#define KEY_P 0x70
#define KEY_Q 0x71
#define KEY_R 0x72
#define KEY_S 0x73
#define KEY_T 0x74
#define KEY_U 0x75
#define KEY_V 0x76
#define KEY_W 0x77
#define KEY_X 0x78
#define KEY_Y 0x79
#define KEY_Z 0x7A

#define KEY_ENTER		0x0A
#define KEY_TAB			0x09
#define KEY_BACKSPACE	0x08

#define KEY_SPACE		0x20
#define KEY_EXCLAMATION	0x21
#define KEY_DOUBLEQUOTE	0x22
#define KEY_HASH		0x23
#define KEY_DOLLAR		0x24
#define KEY_PERCENT		0x25
#define KEY_AMPERSAND	0x26
#define KEY_QUOTE		0x27
#define KEY_LEFTPARENTHESIS		0x28
#define KEY_RIGHTPARENTHESIS	0x29
#define KEY_ASTERISK	0x2A
#define KEY_PLUS		0x2B
#define KEY_COMMA		0x2C
#define KEY_MINUS		0x2D
#define KEY_PERIOD		0x2E
#define KEY_FORWARDSLASH	0x2F
#define KEY_COLON		0x3A
#define KEY_SEMICOLON	0x3B
#define KEY_LESS		0x3C
#define KEY_EQUAL		0x3D
#define KEY_GREATER		0x3E
#define KEY_QUESTION	0x3F
#define KEY_AT			0x40
#define KEY_LEFTBRACKET	0x5B
#define KEY_BACKSLASH	0x5C
#define KEY_RIGHTBRACKET	0x5D
#define KEY_CARRET		0x5E
#define KEY_UNDERSCORE	0x5F
#define KEY_GRAVE		0x60
#define KEY_LEFTCURL	0x7B
#define KEY_BAR			0x7C
#define KEY_RIGHTCURL	0x7D
#define KEY_TILDE		0x7E

/** END ASCII KEYS **/

/** BEGIN OTHER KEYS **/

#define KEY_UNKNOWN	0x0000
#define KEY_INVALID	0x0000

#define KEY_ESCAPE	0x1000
#define KEY_F1		0x1001
#define KEY_F2		0x1002
#define KEY_F3		0x1003
#define KEY_F4		0x1004
#define KEY_F5		0x1005
#define KEY_F6		0x1006
#define KEY_F7		0x1007
#define KEY_F8		0x1008
#define KEY_F9		0x1009
#define KEY_F10		0x100A
#define KEY_F11		0x100B
#define KEY_F12		0x100C

#define KEY_LSHIFT	0x2000
#define KEY_LCTRL	0x2001
#define KEY_LALT	0x2002
#define KEY_RSHIFT	0x2003
#define KEY_RCTRL	0x2004
#define KEY_RALT	0x2005
#define KEY_LWIN	0x2006
#define KEY_RWIN	0x2007

#define KEY_CAPSLOCK	0x3000
#define KEY_NUMLOCK		0x3001
#define KEY_SCROLLLOCK	0x3002

#define KEY_INSERT		0x4000
#define KEY_DELETE		0x4001
#define KEY_HOME		0x4002
#define KEY_END			0x4003
#define KEY_PAGEUP		0x4004
#define KEY_PAGEDOWN	0x4005
#define KEY_PRINTSCREEN	0x4006
#define KEY_PAUSE		0x4007

#define KEY_UP		0x5000
#define KEY_DOWN	0x5001
#define KEY_LEFT	0x5002
#define KEY_RIGHT	0x5003

/** END OTHER KEYS **/

/** BEGIN SCAN CODE SETS **/

static const uint16_t xt_scan[] = {
	KEY_UNKNOWN, KEY_ESCAPE,
	KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
	KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_TAB,
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,
	KEY_LEFTBRACKET, KEY_RIGHTBRACKET, KEY_ENTER, KEY_LCTRL,
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L,
	KEY_SEMICOLON, KEY_QUOTE, KEY_GRAVE, KEY_LSHIFT, KEY_BACKSLASH,
	KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M,
	KEY_COMMA, KEY_PERIOD, KEY_FORWARDSLASH, KEY_RSHIFT,
	KEY_ASTERISK, KEY_RALT, KEY_SPACE, KEY_CAPSLOCK,
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,
	KEY_F9, KEY_F10, KEY_NUMLOCK, KEY_SCROLLLOCK, KEY_HOME,

	KEY_UP, KEY_PAGEUP, KEY_DOWN, KEY_PAGEDOWN, KEY_INSERT, KEY_DELETE,
	KEY_UNKNOWN, KEY_UNKNOWN, KEY_UNKNOWN, KEY_F11, KEY_F12
};

/** END SCAN CODE SETS **/

#endif
