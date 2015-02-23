#ifndef I386_IDT_H
#define I386_IDT_H

#include <stdint.h>

// interrupt descriptor magic
#define IDT_KERNEL_INT  0x8E
#define IDT_KERNEL_TRAP 0x8F
#define IDT_KERNEL_NULL 0x00

#define IDT_NUM_ENTRIES 256

//                 IDT Entry
// 31                  15                  0
// +-------------------+-------------------+
// |      selector     |      offset       |
// |       0:15        |       0:15        |
// +-------------------+-------------------+
//
// 63                  47                 32
// +-------------------+-+--+----+---------+
// |      offset       |P|DP|type|    0    |
// |       16:31       | |L |    |         |
// +-------------------+-+--+----+---------+

typedef union
{
	struct
	{
		uint16_t offset_low;
		uint16_t selector;
		uint8_t zero;
		uint8_t types;
		uint16_t offset_hi;
	} fields;
	uint64_t bits;
} __attribute__((packed)) idt_entry;


typedef struct
{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) idt_ptr;

idt_ptr iptr;

// registers an isr in the idt
void idt_register_isr(uint8_t interrupt, void (*func_ptr));
// clears an isr in the idt
void idt_clear_isr(uint8_t interrupt);

// sets up the table
// this does not register any ISRs
void idt_init();

// calls the lidt asm instruction
extern void idt_load();

#endif
