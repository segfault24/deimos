#ifndef I386_PS2_H
#define I386_PS2_H

#include <stddef.h>
#include <stdint.h>
#include <i386/ps2_codes.h>
#include <i386/ps2_keys.h>

void ps2_init();

uint8_t ps2_test();
void ps2_reset();
//void ps2_enable_kbd();
//void ps2_disable_kbd();

uint8_t ps2_get_scan();
void ps2_clr_scan();
uint8_t ps2_get_key();
void ps2_clr_key();

uint8_t ps2_is_ascii_printable(uint8_t key);
char ps2_key_to_ascii(uint8_t key);

uint8_t ps2_get_alt();
uint8_t ps2_get_ctrl();
uint8_t ps2_get_shift();
uint8_t ps2_get_numlock();
uint8_t ps2_get_capslock();
uint8_t ps2_get_scrolllock();

#endif


