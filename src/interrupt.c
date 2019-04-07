#include "interrupt.h"
#include "irq.h"
#include "io.h"

#define IRQ_MASTER(N, handler) \
	void interrupt_irq##N() { \
		asm volatile( \
			"pusha\n" \
			"call "#handler"\n" \
			"mov $0x20, %eax\n" \
			"outb %al, $0x20\n" \
			"popa\n" \
			"iret\n" \
		); \
		__builtin_unreachable(); \
	}
	
#define IRQ_SLAVE(N, handler) \
	void interrupt_irq##N() { \
		asm volatile( \
			"pusha\n" \
			"call "#handler"\n" \
			"mov $0x20, %eax\n" \
			"outb %al, $0xA0\n" \
			"outb %al, $0x20\n" \
			"popa\n" \
			"iret\n" \
		); \
		__builtin_unreachable(); \
	}

#define IRQ_ENTRY(addr) (struct idt_entry){ addr & 0xffff, 0x08, 0, 0x8e, addr >> 16 }

struct idt_entry idt[286];

void interrupt_init()
{
	io_outb(0x20, 0x11);
	io_outb(0xA0, 0x11);
	io_outb(0x21, 0x20);
	io_outb(0xA1, 0x28);
	io_outb(0x21, 0x04);
	io_outb(0xA1, 0x02);
	io_outb(0x21, 0x01);
	io_outb(0xA1, 0x01);
	io_outb(0x21, 0x0);
	io_outb(0xA1, 0x0);
	
	idt[32] = IRQ_ENTRY((uint32_t)interrupt_irq0);
	idt[33] = IRQ_ENTRY((uint32_t)interrupt_irq1);
	idt[34] = IRQ_ENTRY((uint32_t)interrupt_irq2);
	idt[35] = IRQ_ENTRY((uint32_t)interrupt_irq3);
	idt[36] = IRQ_ENTRY((uint32_t)interrupt_irq4);
	idt[37] = IRQ_ENTRY((uint32_t)interrupt_irq5);
	idt[38] = IRQ_ENTRY((uint32_t)interrupt_irq6);
	idt[39] = IRQ_ENTRY((uint32_t)interrupt_irq7);
	idt[40] = IRQ_ENTRY((uint32_t)interrupt_irq8);
	idt[41] = IRQ_ENTRY((uint32_t)interrupt_irq9);
	idt[42] = IRQ_ENTRY((uint32_t)interrupt_irq10);
	idt[43] = IRQ_ENTRY((uint32_t)interrupt_irq11);
	idt[44] = IRQ_ENTRY((uint32_t)interrupt_irq12);
	idt[45] = IRQ_ENTRY((uint32_t)interrupt_irq13);
	idt[46] = IRQ_ENTRY((uint32_t)interrupt_irq14);
	idt[47] = IRQ_ENTRY((uint32_t)interrupt_irq15);
	
	struct idtr reg = { sizeof(idt) - 1, (uint32_t)idt };
	
	asm volatile(
		"lidt %0\n"
		"sti\n"
		:
		: "m"(reg)
	);
}

IRQ_MASTER(0, irq_handler0)
IRQ_MASTER(1, irq_handler1)
IRQ_MASTER(2, irq_handler2)
IRQ_MASTER(3, irq_handler3)
IRQ_MASTER(4, irq_handler4)
IRQ_MASTER(5, irq_handler5)
IRQ_MASTER(6, irq_handler6)
IRQ_MASTER(7, irq_handler7)
IRQ_SLAVE(8, irq_handler8)
IRQ_SLAVE(9, irq_handler9)
IRQ_SLAVE(10, irq_handler10)
IRQ_SLAVE(11, irq_handler11)
IRQ_SLAVE(12, irq_handler12)
IRQ_SLAVE(13, irq_handler13)
IRQ_SLAVE(14, irq_handler14)
IRQ_SLAVE(15, irq_handler15)
