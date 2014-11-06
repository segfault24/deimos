#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <stdint.h>
#include <stddef.h>

#define VGA_BLACK 0
#define VGA_BLUE 1
#define VGA_GREEN 2
#define VGA_CYAN 3
#define VGA_RED 4
#define VGA_MAGENTA 5
#define VGA_BROWN 6
#define VGA_LIGHT_GREY 7
#define VGA_DARK_GREY 8
#define VGA_LIGHT_BLUE 9
#define VGA_LIGHT_GREEN 10
#define VGA_LIGHT_CYAN 11
#define VGA_LIGHT_RED 12
#define VGA_LIGHT_MAGENTA 13
#define VGA_LIGHT_BROWN 14
#define VGA_WHTIE 15

#define VGA_BASE_ADDR 0x000B8000
#define VGA_INDEX_REG 0x03D4
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void tty_setcolor(uint8_t bg, uint8_t fg);
void tty_clear();
void tty_init();

void tty_putchar(char c);
char tty_getchar();

void tty_puts(const char* str);
void tty_puti(uint32_t i);

#endif
