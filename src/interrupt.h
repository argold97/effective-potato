#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

struct __attribute__((packed)) idtr {
		uint16_t size;
		uint32_t base;
};

struct __attribute__((packed)) idt_entry {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_high;
};

void interrupt_init();

inline void interrupt_enable()
{
	asm volatile("sti");
}

inline void interrupt_disable()
{
	asm volatile("cli");
}

void interrupt_irq0();
void interrupt_irq1();
void interrupt_irq2();
void interrupt_irq3();
void interrupt_irq4();
void interrupt_irq5();
void interrupt_irq6();
void interrupt_irq7();
void interrupt_irq8();
void interrupt_irq9();
void interrupt_irq10();
void interrupt_irq11();
void interrupt_irq12();
void interrupt_irq13();
void interrupt_irq14();
void interrupt_irq15();

#endif
