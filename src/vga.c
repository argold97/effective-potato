#include "vga.h"
#include "string.h"
#include <stdint.h>

static uint8_t vga_current_color = 0;
static uint16_t* vga_cursor = (uint16_t*)VGA_BUF;

inline uint8_t vga_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

inline uint16_t vga_char(char c, uint8_t color)
{
	return (uint16_t) c | (uint16_t) color << 8;
}

inline uint16_t* vga_at(size_t row, size_t col)
{
	return (uint16_t*) (VGA_BUF + (VGA_WIDTH * row + col) * sizeof(uint16_t));
}

void vga_set_color(enum vga_color fg, enum vga_color bg)
{
	vga_current_color = vga_color(fg, bg);
}

void vga_putchar_at(char c, size_t row, size_t col)
{
	*vga_at(row, col) = vga_char(c, vga_current_color);
}

void vga_scroll()
{
	size_t row_size = VGA_WIDTH * sizeof(uint16_t); 
	memcpy((void*)VGA_BUF, (void*)VGA_BUF + row_size, row_size * (VGA_HEIGHT - 1));
	memset((void*)VGA_BUF + row_size * (VGA_HEIGHT - 1), 0, row_size);
	
	if(vga_cursor > (uint16_t*)VGA_BUF + VGA_WIDTH)
		vga_cursor -= VGA_WIDTH;
}

void vga_putchar(char c)
{
	if(c == '\n') {
		vga_cursor = (uint16_t*) (VGA_BUF + (((long unsigned int)vga_cursor - VGA_BUF) / (VGA_WIDTH * sizeof(uint16_t)) + 1) * VGA_WIDTH * sizeof(uint16_t));
	} else {
		*vga_cursor++ = vga_char(c, vga_current_color);
	}
	if(vga_cursor >= ((uint16_t*)VGA_BUF) + VGA_WIDTH * VGA_HEIGHT)
		vga_scroll();
}

void vga_write(const char* str, size_t n)
{
	while(n > 0) {
		vga_putchar(*str++);
		n--;
	}
}

void vga_print(const char* str)
{
	char c;
	while((c = *str++))
		vga_putchar(c);
}

void vga_clear()
{
	memset((void*)VGA_BUF, 0, VGA_WIDTH * VGA_HEIGHT * sizeof(uint16_t));
	vga_cursor = (uint16_t*)VGA_BUF;
}

void vga_printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	vga_vprintf(fmt, args);
}

void vga_vprintf(const char* fmt, va_list args)
{
	while(*fmt != '\0')
	{
		if(*fmt == '\\') {
			++fmt;
			if(*fmt == 'n')
				vga_putchar('\n');
			else if(*fmt == '\\' || *fmt == '%')
				vga_putchar(*fmt);
			else if(*fmt == '\0')
				break;
		} else if(*fmt == '%') {
			++fmt;
			uint32_t val = va_arg(args, uint32_t);
			if(*fmt == 'c') {
				vga_putchar((char)val);
			}
			else if(*fmt == 'd') {
				char buf[11];
				vga_print(itoa(val, buf));
			} else if(*fmt == 'x') {
				char buf[9];
				vga_print(itoa_hex(val, buf));
			} else if(*fmt == 's') {
				vga_print((char*)val);
			} else if(*fmt == '\0')
				break;
		} else
			vga_putchar(*fmt);
		++fmt;
	}
}
