#ifndef IO_H
#define IO_H

#include <stdint.h>

inline void io_outb(uint16_t port, uint8_t val)
{
	asm volatile(
		"outb %0, %1\n"
		:
		: "a"(val), "Nd"(port)
	);
}

inline void io_outw(uint16_t port, uint16_t val)
{
	asm volatile(
		"outw %0, %1\n"
		:
		: "a"(val), "Nd"(port)
	);
}

inline void io_outl(uint16_t port, uint32_t val)
{
	asm volatile(
		"outl %0, %1\n"
		:
		: "a"(val), "Nd"(port)
	);
}

inline uint8_t io_inb(uint16_t port)
{	
	uint8_t val;
	asm volatile(
		"inb %1, %0\n"
		: "=a"(val)
		: "Nd"(port)
	);
	return val;
}

inline uint16_t io_inw(uint16_t port)
{	
	uint16_t val;
	asm volatile(
		"inw %1, %0\n"
		: "=a"(val)
		: "Nd"(port)
	);
	return val;
}

inline uint32_t io_inl(uint16_t port)
{	
	uint32_t val;
	asm volatile(
		"inl %1, %0\n"
		: "=a"(val)
		: "Nd"(port)
	);
	return val;
}

inline void io_wait()
{
	asm volatile(
		"outb %al, $0x80\n"
	);
}

#endif
