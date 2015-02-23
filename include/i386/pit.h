#ifndef I386_PIT_H
#define I386_PIT_H

#define PIT_CH0_DATA 0x40
#define PIT_CH2_DATA 0x42
#define PIT_CMD 0x43

// set channel 0 to mode 3 (square wave)
#define PIT_CH0_CFG 0x36
// with a reload value of 0xFFFF
// which is a rate of 18.2065Hz
#define PIT_CH0_RLDH 0xFF
#define PIT_CH0_RLDL 0xFF

//void pit_beep();

void pit_init();

#endif
