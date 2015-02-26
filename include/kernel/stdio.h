#ifndef KERNEL_STDIO_H
#define KERNEL_STDIO_H

int getchar();

int putchar(int character);
int puts(const char* str);
void printf(const char* format, ...);

#endif
