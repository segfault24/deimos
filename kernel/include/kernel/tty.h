#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

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

#define VGA_BASE_ADDR 0x0B8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void term_setcolor(uint8_t bg, uint8_t fg);
void term_putchar(char c);
void term_puts(const char* str);
void term_scroll();
void term_clear();
void term_init();

#endif
