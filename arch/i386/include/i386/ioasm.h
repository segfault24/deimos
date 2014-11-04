#ifndef I386_IOASM_H
#define I386_IOASM_H

#include <stdint.h>

static inline void iowait()
{
	__asm__ volatile ( "outb %%al, $0x80" : : "a"(0) );
}

static inline void outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
	return ret;
}

//static inline void outw(uint16_t port, uint16_t val);
//static inline void outl(uint16_t port, uint32_t val);

//static inline uint16_t inw(uint16_t port);
//static inline uint32_t inl(uint16_t port);

#endif