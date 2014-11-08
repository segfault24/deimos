#ifndef I386_PS2_CODES_H
#define I386_PS2_CODES_H

#include <stdint.h>

/***************************/
/*      Command Codes      */
/***************************/

// keyboard encoder ports
#define PS2_KBD_ENC_CMD_REG		0x60
#define PS2_KBD_ENC_DATA_BUF	0x60
// keyboard encoder command codes
#define PS2_KBD_ENC_SETLEDS		0xED
#define PS2_KBD_ENC_ECHO		0xEE
#define PS2_KBD_ENC_SETSCAN		0xF0
#define PS2_KBD_ENC_SETID		0xF2
#define PS2_KBD_ENC_SETREPEAT	0xF3
#define PS2_KBD_ENC_ENABLEKBD	0xF4
#define PS2_KBD_ENC_RST0		0xF5
#define PS2_KBD_ENC_RST1		0xF6
#define PS2_KBD_ENC_ALLREPEAT	0xF7
#define PS2_KBD_ENC_ALLMAKEBRK	0xF8
#define PS2_KBD_ENC_ALLMAKE		0xF9
#define PS2_KBD_ENC_ALLALL		0xFA
#define PS2_KBD_ENC_SNGLREPEAT	0xFB
#define PS2_KBD_ENC_SNGLMAKEBRK	0xFC
#define PS2_KBD_ENC_SNGLEBRK	0xFD
#define PS2_KBD_ENC_RESEND		0xFE
#define PS2_KBD_ENC_RSTTEST		0xFF

// keyboard controller ports
#define PS2_KBD_CTRL_CMD_REG	0x64
#define PS2_KBD_CTRL_STATUS_REG	0x64
// keyboard controller status register bits
#define PS2_KBD_CTRL_OUTBUFRDY	0x01
#define PS2_KBD_CTRL_INBUFFULL	0x02
#define PS2_KBD_CTRL_SYSRDY		0x04
#define PS2_KBD_CTRL_CMDDATA	0x08
#define PS2_KBD_CTRL_UNLOCKED	0x10
#define PS2_KBD_CTRL_AUXBUF		0x20
#define PS2_KBD_CTRL_TIMEOUT	0x40
#define PS2_KBD_CTRL_PARITYERR	0x80
// keyboard controller command codes
#define PS2_KBD_CTRL_READCMD	0x20
#define PS2_KBD_CTRL_WRITECMD	0x60
#define PS2_KBD_CTRL_SELFTEST	0xAA
#define PS2_KBD_CTRL_IFACETEST	0xAB
#define PS2_KBD_CTRL_DISABLE	0xAD
#define PS2_KBD_CTRL_ENABLE		0xAE
#define PS2_KBD_CTRL_READIN		0xC0
#define PS2_KBD_CTRL_READOUT	0xD0
#define PS2_KBD_CTRL_WRITEOUT	0xD1
#define PS2_KBD_CTRL_READTEST	0xE0
#define PS2_KBD_CTRL_RESET		0xFE
#define PS2_KBD_CTRL_DSBLMOUSE	0xA7
#define PS2_KBD_CTRL_ENBLMOUSE	0xA8
#define PS2_KBD_CTRL_TESTMOUSE	0xA9
#define PS2_KBD_CTRL_WRITEMOUSE	0xD4
// keyboard controller error codes
// TODO: add these

#endif


