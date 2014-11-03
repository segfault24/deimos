#ifndef I386_PIT_H
#define I386_PIT_H

#include <stddef.h>
#include <stdint.h>

#define PIT_CH0_DATA 0x40
#define PIT_CH1_DATA 0x41
#define PIT_CH2_DATA 0x42
#define PIT_CMD 0x43

void pit_init();

#endif
