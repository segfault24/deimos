#ifndef KERNEL_PANIC_H
#define KERNEL_PANIC_H

#include <stddef.h>
#include <stdint.h>

void kernel_panic(char* message);

#endif
