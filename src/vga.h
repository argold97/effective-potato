#ifndef VGA_H
#define VGA_H

#include <stddef.h>
#include <stdarg.h>

#define VGA_BUF 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void vga_clear();
void vga_set_color(enum vga_color fg, enum vga_color bg);
void vga_putchar_at(char c, size_t row, size_t col);
void vga_putchar(char c);
void vga_write(const char* str, size_t n);
void vga_print(const char* str);
void vga_printf(const char* fmt, ...);
void vga_vprintf(const char* fmt, va_list args);
void vga_scroll();

#endif
