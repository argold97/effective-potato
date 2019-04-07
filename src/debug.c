#include "debug.h"
#include "vga.h"
#include <stdarg.h>

void debug_putchar(char c)
{
	vga_putchar(c);
}

void debug_printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	vga_vprintf(fmt, args);
}

void debug_print_gdt_entry(struct gdtr* reg, size_t index)
{
	struct gdt_entry* entry = (struct gdt_entry*)(reg->base + index * sizeof(struct gdt_entry));
	uint32_t limit = (uint32_t)entry->limit_low | ((uint32_t)entry->limit_high_flags & 0xf) << 16;
	uint32_t base = (uint32_t)entry->base_low | (uint32_t)entry->base_mid << 16 | (uint32_t)entry->base_high << 24;
	
	vga_printf("Entry #%d\nLimit:  %x\nBase:   %x\nAccess: %x\nFlags:  %x\n\n", index, limit, base, entry->access, (entry->limit_high_flags >> 4) & 0xf);
}