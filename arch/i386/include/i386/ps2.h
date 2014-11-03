#ifndef I386_PS2_H
#define I386_PS2_H

#include <stddef.h>
#include <stdint.h>

#define PS2_REG_DATA 0x60
#define PS2_REG_STATUS 0x64
#define PS2_REG_CMD 0x64

//uint8_t ps2_write_cmd();
//uint8_t ps2_read_status();

void ps2_init();

#endif
